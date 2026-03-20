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
R"(\def\Xi{Ξ})",
R"(\def\Pi{Π})",
R"(\def\Sigma{Σ})",
R"(\def\Upsilon{ϒ})",
R"(\def\Phi{Φ})",
R"(\def\Psi{Ψ})",
R"(\def\Omega{Ω})",

R"(\def\nless{≮})"
R"(\def\leq{≤})"
R"(\def\leqslant{⩽})"
R"(\def\nleq{≰})"
R"(\def\nleqslant{⪇})"
R"(\def\prec{≺})"
R"(\def\nprec{⊀})"
R"(\def\preceq{⪯})"
R"(\def\npreceq{⋠})"
R"(\def\ll{≪})"
R"(\def\lll{⋘})"
R"(\def\subset{⊂})"
R"(\def\subseteq{⊆})"
R"(\def\nsubseteq{⊈})"
R"(\def\sqsubset{⊏})"
R"(\def\sqsubseteq{⊑})"

R"(\def\ngtr{≯})"
R"(\def\geq{≥})"
R"(\def\geqslant{⩾})"
R"(\def\ngeq{≱})"
R"(\def\ngeqslant{⪈})"
R"(\def\succ{≻})"
R"(\def\nsucc{⊁})"
R"(\def\succeq{⪰})"
R"(\def\nsucceq{⋡})"
R"(\def\gg{≫})"
R"(\def\ggg{⋙})"
R"(\def\supset{⊃})"
R"(\def\supseteq{⊇})"
R"(\def\nsupseteq{⊉})"
R"(\def\sqsupset{⊐})"
R"(\def\sqsupseteq{⊒})"

R"(\def\doteq{≐})"
R"(\def\equiv{≡})"
R"(\def\approx{≈})"
R"(\def\cong{≅})"
R"(\def\simeq{≃})"
R"(\def\sim{∼})"
R"(\def\propto{∝})"
R"(\def\neq{≠})",

R"(\def\parallel{∥})",
R"(\def\nparallel{∦})",
R"(\def\asymp{≍})",
R"(\def\bowtie{⋈})",
R"(\def\vdash{⊢})",
R"(\def\dashv{⊣})",
R"(\def\in{∈})",
R"(\def\ni{∋})",
R"(\def\smile{⌣})",
R"(\def\frown{⌢})",
R"(\def\models{⊨})",
R"(\def\notin{∉})",
R"(\def\perp{⊥})",
R"(\def\mid{\mathrel{|}})",

R"(\def\pm{±})",
R"(\def\mp{∓})",
R"(\def\times{×})",
R"(\def\div{÷})",
R"(\def\ast{∗})",
R"(\def\star{⋆})",
R"(\def\dagger{†})",
R"(\def\ddagger{‡})",
R"(\def\cap{∩})",
R"(\def\cup{∪})",
R"(\def\uplus{⊎})",
R"(\def\sqcap{⊓})",
R"(\def\sqcup{⊔})",
R"(\def\vee{∨})",
R"(\def\wedge{∧})",
R"(\def\cdot{⋅})",
R"(\def\diamond{⋄})",
R"(\def\bigtriangleup{△})",
R"(\def\bigtriangledown{▽})",
R"(\def\triangleleft{◃})",
R"(\def\triangleright{▹})",
R"(\def\bigcirc{○})",
R"(\def\bullet{•})",
R"(\def\wr{≀})",
R"(\def\oplus{⊕})",
R"(\def\ominus{⊖})",
R"(\def\otimes{⊗})",
R"(\def\oslash{⊘})",
R"(\def\odot{⊙})",
R"(\def\circ{∘})",
R"(\def\setminus{∖})",
R"(\def\amalg{∐})",
R"(\def\lcontr{\mathbin{⌋}})",
R"(\def\rcontr{\mathbin{⌊}})",

R"(\def\varnothing{∅})",
R"(\def\exists{∃})",
R"(\def\nexists{∄})",
R"(\def\forall{∀})",
R"(\def\neg{¬})",
R"(\def\Longrightarrow{⟹})",
R"(\def\implies{⟹})",
R"(\def\Rightarrow{⇒})",
R"(\def\Longleftarrow{⟸})",
R"(\def\Leftarrow{⇐})",
R"(\def\iff{⟺})",
R"(\def\Leftrightarrow{⇔})",
R"(\def\top{⊤})",
R"(\def\bot{⊥})",

R"(\def\angle{∠})",
R"(\def\triangle{△})",
R"(\def\measuredangle{∡})",
R"(\def\square{□})",
R"(\def\ncong{≇})",
R"(\def\nsim{≁})",

R"(\def\lceil{⌈})",
R"(\def\rceil{⌉})",
R"(\def\ulcorner{⌜})",
R"(\def\urcorner{⌝})",
R"(\def\langle{⟨})",
R"(\def\rangle{⟩})",
R"(\def\lfloor{⌊})",
R"(\def\rfloor{⌋})",
R"(\def\llcorner{⌞})",
R"(\def\lrcorner{⌟})",

R"(\def\rightarrow{→})",
R"(\def\to{→})",
R"(\def\mapsto{↦})",
R"(\def\leftarrow{←})",
R"(\def\gets{←})",
R"(\def\longrightarrow{⟶})",
R"(\def\longmapsto{⟼})",
R"(\def\longleftarrow{⟵})",
R"(\def\uparrow{↑})",
R"(\def\downarrow{↓})",
R"(\def\updownarrow{↕})",
R"(\def\Uparrow{⇑})",
R"(\def\Downarrow{⇓})",
R"(\def\Updownarrow{⇕})",

R"(\def\partial{∂})",
R"(\def\eth{ð})",
R"(\def\hbar{ℏ})",
R"(\def\imath{𝚤})",
R"(\def\jmath{𝚥})",
R"(\def\ell{ℓ})",
R"(\def\Re{ℜ})",
R"(\def\Im{ℑ})",
R"(\def\wp{℘})",
R"(\def\nabla{∇})",
R"(\def\Box{◻})",
R"(\def\infty{∞})",

R"(\def\aleph{ℵ})",
R"(\def\beth{ℶ})",
R"(\def\gimel{ℷ})",

R"(\def\arg{\mathop{\text{arg}}})",
R"(\def\deg{\mathop{\text{deg}}})",
R"(\def\det{\mathop{\text{det}}})",
R"(\def\dim{\mathop{\text{dim}}})",
R"(\def\exp{\mathop{\text{exp}}})",
R"(\def\gcd{\mathop{\text{gcd}}})",
R"(\def\hom{\mathop{\text{hom}}})",
R"(\def\inf{\mathop{\text{inf}}})",
R"(\def\ker{\mathop{\text{ker}}})",
R"(\def\lg{\mathop{\text{lg}}})",
R"(\def\lim{\mathop{\text{lim}}})",
R"(\def\liminf{\mathop{\text{liminf}}})",
R"(\def\limsup{\mathop{\text{limsup}}})",
R"(\def\ln{\mathop{\text{ln}}})",
R"(\def\log{\mathop{\text{log}}})",
R"(\def\max{\mathop{\text{max}}})",
R"(\def\min{\mathop{\text{min}}})",
R"(\def\Pr{\mathop{\text{Pr}}})",
R"(\def\sup{\mathop{\text{sup}}})",

R"(\def\sin{\mathop{\text{sin}}})",
R"(\def\cos{\mathop{\text{cos}}})",
R"(\def\tan{\mathop{\text{tan}}})",
R"(\def\csc{\mathop{\text{csc}}})",
R"(\def\sec{\mathop{\text{sec}}})",
R"(\def\cot{\mathop{\text{cot}}})",
R"(\def\arcsin{\mathop{\text{arcsin}}})",
R"(\def\arccos{\mathop{\text{arccos}}})",
R"(\def\arctan{\mathop{\text{arctan}}})",
R"(\def\arccsc{\mathop{\text{arccsc}}})",
R"(\def\arcsec{\mathop{\text{arcsec}}})",
R"(\def\arccot{\mathop{\text{arccot}}})",
R"(\def\sinh{\mathop{\text{sinh}}})",
R"(\def\cosh{\mathop{\text{cosh}}})",
R"(\def\tanh{\mathop{\text{tanh}}})",
R"(\def\csch{\mathop{\text{csch}}})",
R"(\def\sech{\mathop{\text{sech}}})",
R"(\def\coth{\mathop{\text{coth}}})",
R"(\def\arcsinh{\mathop{\text{arcsinh}}})",
R"(\def\arccosh{\mathop{\text{arccosh}}})",
R"(\def\arctanh{\mathop{\text{arctanh}}})",
R"(\def\arccsch{\mathop{\text{arccsch}}})",
R"(\def\arcsech{\mathop{\text{arcsech}}})",
R"(\def\arccoth{\mathop{\text{arccoth}}})",
};

}
