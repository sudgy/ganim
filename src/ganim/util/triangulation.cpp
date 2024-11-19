#include "triangulation.hpp"

#include <random>
#include <list>

#include "ganim/ga/conversions.hpp"

// https://www.cs.umd.edu/class/spring2020/cmsc754/Lects/lect05-triangulate.pdf

namespace ganim {

// Name lookup for operators works a bit differently for static functions vs.
// anonymous namespaces, and this won't be found if it's in an anomymous
// namespace
static auto operator<=>(vga2::Vector v1, vga2::Vector v2)
{
    auto x1 = v1.blade_project<vga2::e1>();
    auto x2 = v2.blade_project<vga2::e1>();
    auto y1 = v1.blade_project<vga2::e2>();
    auto y2 = v2.blade_project<vga2::e2>();
    if (x1 == x2) return y1 <=> y2;
    else return x1 <=> x2;
}

namespace {
    constexpr unsigned invalid = -1;
    struct Triangulator {
        const std::vector<vga2::Vector>& M_polygon;
        std::vector<std::vector<unsigned>> M_monotone_polygons;
        std::vector<unsigned> M_triangulation;
        std::vector<unsigned> M_sweep_order;
        struct Edge {
            unsigned p1 = invalid;
            unsigned p2 = invalid;
            unsigned helper = invalid;
            bool helper_merge = false;
        };
        std::vector<Edge> M_sweep_edges;
        std::vector<std::pair<unsigned, unsigned>> M_diagonals;
        bool M_ccw = false;

        Triangulator(const std::vector<vga2::Vector>& polygon)
        : M_polygon(polygon)
        {
            auto area = 0.0;
            for (int i = 0; i < ssize(polygon); ++i) {
                auto this_part = polygon[i] ^ polygon[(i + 1) % ssize(polygon)];
                area += this_part.blade_project<vga2::e12>();
            }
            M_ccw = area > 0;
            M_triangulation.reserve(M_polygon.size() * 3);
            M_sweep_order.reserve(M_polygon.size());
            for (unsigned i = 0; i < M_polygon.size(); ++i) {
                M_sweep_order.push_back(i);
            }
            std::ranges::sort(
                M_sweep_order,
                {},
                [&](unsigned i){return M_polygon[i];}
            );
        }
        unsigned next_vertex(unsigned v)
        {
            return (v + 1) % M_polygon.size();
        }
        unsigned previous_vertex(unsigned v)
        {
            return v == 0 ? M_polygon.size() - 1 : v - 1;
        }
        void find_diagonals()
        {
            for (auto v : M_sweep_order) {
                unsigned on_edge1 = invalid;
                unsigned on_edge2 = invalid;
                unsigned below_edge = invalid;
                for (int i = 0; i < ssize(M_sweep_edges); ++i) {
                    if (M_sweep_edges[i].p2 == v) {
                        if (on_edge1 == invalid) {
                            on_edge1 = i;
                        }
                        else {
                            on_edge2 = i;
                            break;
                        }
                    }
                    else {
                        if (on_edge1 != invalid) break;
                        auto p = vga2_to_pga2(M_polygon[v]);
                        auto a = vga2_to_pga2(M_polygon[M_sweep_edges[i].p1]);
                        auto b = vga2_to_pga2(M_polygon[M_sweep_edges[i].p2]);
                        auto intersect = (p | pga2::e2) ^ (a & b);
                        // I'm pretty sure intersect's e12 component will always
                        // be nonzero
                        intersect /= intersect.blade_project<pga2::e12>();
                        auto edge_y = intersect.blade_project<pga2::e01>();
                        if (edge_y < M_polygon[v].blade_project<vga2::e2>()) {
                            below_edge = i;
                            break;
                        }
                    }
                }
                if (on_edge2 == invalid) {
                    if (on_edge1 == invalid) {
                        if (below_edge == invalid or below_edge % 2 == 0) {
                            start_vertex(v, below_edge);
                        }
                        else {
                            split_vertex(v, below_edge);
                        }
                    }
                    else {
                        chain_vertex(v, on_edge1);
                    }
                }
                else {
                    if (on_edge1 % 2 == 0) {
                        end_vertex(v, on_edge1);
                    }
                    else {
                        merge_vertex(v, on_edge1);
                    }
                }
            }
        }
        void fix_up(Edge& e, unsigned v)
        {
            if (e.helper_merge) {
                add_diagonal(e.helper, v);
            }
        }
        std::pair<Edge*, Edge*> new_sweep_edges(unsigned v, unsigned below_edge)
        {
            auto edge1 = Edge();
            auto edge2 = Edge();
            edge1.p1 = v;
            edge2.p1 = v;
            auto v1 = next_vertex(v);
            auto v2 = previous_vertex(v);
            auto y1 = M_polygon[v1].blade_project<vga2::e2>();
            auto y2 = M_polygon[v2].blade_project<vga2::e2>();
            if (y1 < y2) {
                edge1.p2 = v2;
                edge2.p2 = v1;
            }
            else {
                edge1.p2 = v1;
                edge2.p2 = v2;
            }
            if (below_edge == invalid) {
                below_edge = M_sweep_edges.size();
                M_sweep_edges.push_back(edge1);
                M_sweep_edges.push_back(edge2);
            }
            else {
                M_sweep_edges.insert(M_sweep_edges.begin() + below_edge, edge2);
                M_sweep_edges.insert(M_sweep_edges.begin() + below_edge, edge1);
            }
            return {&M_sweep_edges[below_edge], &M_sweep_edges[below_edge+1]};
        }
        void start_vertex(unsigned v, unsigned below_edge)
        {
            auto [edge1, _] = new_sweep_edges(v, below_edge);
            edge1->helper = v;
        }
        void split_vertex(unsigned v, unsigned below_edge)
        {
            add_diagonal(v, M_sweep_edges[below_edge-1].helper);
            auto [_, edge2] = new_sweep_edges(v, below_edge);
            M_sweep_edges[below_edge-1].helper = v;
            edge2->helper = v;
        }
        void chain_vertex(unsigned v, unsigned edge_index)
        {
            auto& edge = M_sweep_edges[edge_index];
            fix_up(edge, v);
            auto v1 = next_vertex(v);
            auto v2 = previous_vertex(v);
            if (edge.p1 == v1) edge.p2 = v2;
            else edge.p2 = v1;
            edge.p1 = v;
            edge.helper = v;
            edge.helper_merge = false;
        }
        void end_vertex(unsigned v, unsigned upper_edge)
        {
            fix_up(M_sweep_edges[upper_edge], v);
            M_sweep_edges.erase(M_sweep_edges.begin() + upper_edge);
            M_sweep_edges.erase(M_sweep_edges.begin() + upper_edge);
        }
        void merge_vertex(unsigned v, unsigned upper_edge)
        {
            fix_up(M_sweep_edges[upper_edge - 1], v);
            fix_up(M_sweep_edges[upper_edge + 1], v);
            M_sweep_edges.erase(M_sweep_edges.begin() + upper_edge);
            M_sweep_edges.erase(M_sweep_edges.begin() + upper_edge);
            M_sweep_edges[upper_edge - 1].helper = v;
            M_sweep_edges[upper_edge - 1].helper_merge = true;
        }
        void add_diagonal(unsigned v1, unsigned v2)
        {
            M_diagonals.emplace_back(std::min(v1, v2), std::max(v1, v2));
        }
        void monotonize()
        {
            std::ranges::sort(
                M_diagonals,
                {},
                [](auto d){return d.second - d.first;}
            );
            M_monotone_polygons.reserve(M_diagonals.size()+1);
            auto& main_poly = M_monotone_polygons.emplace_back();
            main_poly.resize(M_polygon.size());
            std::ranges::iota(main_poly, 0);
            for (auto [d1, d2] : M_diagonals) {
                auto& new_poly = M_monotone_polygons.emplace_back();
                new_poly.push_back(d1);
                auto begin = std::ranges::lower_bound(main_poly, d1) + 1;
                auto end = begin;
                while (*end != d2) {
                    new_poly.push_back(*end);
                    ++end;
                }
                new_poly.push_back(d2);
                main_poly.erase(begin, end);
            }
        }
        void triangulate()
        {
            for (auto& polygon : M_monotone_polygons) {
                triangulate_monotone(polygon);
            }
        }
        void triangulate_monotone(const std::vector<unsigned>& polygon)
        {
            auto proj = [&](unsigned i) {return M_polygon[i];};
            auto min = std::ranges::min_element(polygon, {}, proj);
            auto max = std::ranges::max_element(polygon, {}, proj);
            auto min_pos = int(min - polygon.begin());
            auto max_pos = int(max - polygon.begin());
            auto mid_pos = -1;
            auto sorted = std::vector<std::pair<unsigned, bool>>();
            sorted.reserve(polygon.size());
            if (min_pos < max_pos) {
                for (int i = min_pos; i < max_pos; ++i) {
                    sorted.emplace_back(polygon[i], false);
                }
                for (int i = min_pos - 1; i >= 0; --i) {
                    sorted.emplace_back(polygon[i], true);
                }
                for (int i = ssize(polygon) - 1; i >= max_pos; --i) {
                    sorted.emplace_back(polygon[i], true);
                }
                mid_pos = max_pos - min_pos;
            }
            else {
                for (int i = min_pos; i > max_pos; --i) {
                    sorted.emplace_back(polygon[i], true);
                }
                for (int i = min_pos + 1; i < ssize(polygon); ++i) {
                    sorted.emplace_back(polygon[i], false);
                }
                for (int i = 0; i <= max_pos; ++i) {
                    sorted.emplace_back(polygon[i], false);
                }
                mid_pos = min_pos - max_pos;
            }
            std::ranges::inplace_merge(
                sorted,
                sorted.begin() + mid_pos,
                {},
                [&](auto p){return proj(p.first);}
            );

            auto chain = std::vector<unsigned>();
            // This is actually only up if the original polygon was
            // counterclockwise but it helps to give this a concrete name
            bool chain_up = true;
            for (auto [v, v_up] : sorted) {
                if (chain.size() < 2) {
                    chain.push_back(v);
                    chain_up = v_up;
                }
                else {
                    if (chain_up == v_up) {
                        while (true) {
                            auto i1 = chain.rbegin()[1];
                            auto i2 = chain.back();
                            auto p1 = M_polygon[i1];
                            auto p2 = M_polygon[i2];
                            auto p3 = M_polygon[v];
                            auto outer = (p2 - p1) ^ (p3 - p2);
                            auto orient = outer.blade_project<vga2::e12>();
                            if ((orient > 0) ^ M_ccw ^ v_up) break;
                            add_triangle(i1, i2, v);
                            chain.pop_back();
                            if (chain.size() == 1) break;
                        }
                        chain.push_back(v);
                    }
                    else {
                        for (int i = 0; i < ssize(chain) - 1; ++i) {
                            add_triangle(v, chain[i], chain[i+1]);
                        }
                        chain = {chain.back(), v};
                        chain_up = v_up;
                    }
                }
            }
        }
        void add_triangle(unsigned v1, unsigned v2, unsigned v3)
        {
            auto p1 = M_polygon[v1];
            auto p2 = M_polygon[v2];
            auto p3 = M_polygon[v3];
            auto this_area = ((p2 - p1) ^ (p3 - p1)).blade_project<vga2::e12>();
            // If there exist three collinear points in the polygon you can get
            // a degenerate triangle and we don't want those
            if (this_area == 0) return;
            M_triangulation.push_back(v1);
            if ((this_area > 0) == M_ccw) {
                M_triangulation.push_back(v2);
                M_triangulation.push_back(v3);
            }
            else {
                M_triangulation.push_back(v3);
                M_triangulation.push_back(v2);
            }
        }
    };
}

std::vector<unsigned> triangulate(const std::vector<vga2::Vector>& polygon)
{
    if (polygon.size() < 3) {
        throw std::invalid_argument(
            "When triangulating a polygon, the polygon must have at least three"
            " points!"
        );
    }
    auto triangulator = Triangulator(polygon);
    triangulator.find_diagonals();
    triangulator.monotonize();
    triangulator.triangulate();
    return triangulator.M_triangulation;
}

std::vector<unsigned> triangulate(const std::vector<pga2::Bivector>& polygon)
{
    auto vga_polygon = std::vector<vga2::Vector>();
    vga_polygon.reserve(polygon.size());
    for (auto p : polygon) {
        vga_polygon.emplace_back(pga2_to_vga2(p));
    }
    return triangulate(vga_polygon);
}

}
