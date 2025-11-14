#include <vector>
#include <string>

namespace {

std::vector<std::string_view> macro_input = {
R"(\def\vec#1{\mathaccent →{#1}})",
R"(\def\bvec#1{\mathaccent ⇒{#1}})",
R"(\def\tvec#1{\mathaccent ⇛{#1}})",
R"(\def\hat#1{\mathaccentscale 0.8 ‸{#1}})",

R"(\def\over{\abovewithdelims.. -1pt})",
R"(\def\atop{\abovewithdelims.. 0pt})",
R"(\def\choose{\abovewithdelims() 0pt})",
R"(\def\above{\abovewithdelims.. })",
R"(\def\overwithdelims#1#2{\abovewithdelims#1#2 -1pt})",
R"(\def\atopwithdelims#1#2{\abovewithdelims#1#2 0pt})",

R"(\def\,{\mskip 3mu})",
R"(\def\>{\mskip 4mu})",
R"(\def\;{\mskip 5mu})",
R"(\def\!{\mskip -3mu})",

R"(\def\cdots{\mathinner{⋯}})",
R"(\def\ldots{\mathinner{…}})",

R"(\def\sqrt#1{\radical √{#1}})",

R"(\def\pm{±})",
R"(\def\mp{∓})",
R"(\def\cdot{⋅})",
R"(\def\wedge{∧})",
R"(\def\vee{∨})",
R"(\def\lcontr{\mathbin{⌋}})",
R"(\def\rcontr{\mathbin{⌊}})",
R"(\def\ell{ℓ})",
R"(\def\perp{⊥})",
R"(\def\parallel{∥})",

R"(\def\alpha{𝛼})",
R"(\def\beta{𝛽})",
R"(\def\gamma{𝛾})",
R"(\def\delta{𝛿})",
R"(\def\epsilon{𝜖})",
R"(\def\varepsilon{𝜀})",
R"(\def\zeta{𝜁})",
R"(\def\eta{𝜂})",
R"(\def\theta{𝜃})",
R"(\def\vartheta{𝜗})",
R"(\def\iota{𝜄})",
R"(\def\kappa{𝜅})",
R"(\def\lambda{𝜆})",
R"(\def\mu{𝜇})",
R"(\def\nu{𝜈})",
R"(\def\xi{𝜉})",
R"(\def\pi{𝜋})",
R"(\def\varpi{𝜛})",
R"(\def\rho{𝜌})",
R"(\def\varrho{𝜚})",
R"(\def\sigma{𝜎})",
R"(\def\varsigma{𝜍})",
R"(\def\tau{𝜏})",
R"(\def\upsilon{𝜐})",
R"(\def\phi{𝜙})",
R"(\def\varphi{𝜑})",
R"(\def\chi{𝜒})",
R"(\def\psi{𝜓})",
R"(\def\omega{𝜔})",

R"(\def\Gamma{Γ})",
R"(\def\Delta{Δ})",
R"(\def\Theta{Θ})",
R"(\def\Lambda{Λ})",
R"(\def\xi{Ξ})",
R"(\def\pi{Π})",
R"(\def\sigma{Σ})",
R"(\def\upsilon{ϒ})",
R"(\def\phi{Φ})",
R"(\def\psi{Ψ})",
R"(\def\omega{Ω})",
};

}
