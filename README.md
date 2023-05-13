###My modified version of Barrett reduction

If $n\in[2^{r-1},2^r)$, $a\in[0,2^{2r})$, let $n'=\left\lfloor\dfrac{2^{2r+1}}{n}\right\rfloor$, $a_1=\left\lfloor\dfrac{a}{2^{r-2}}\right\rfloor$, $k=\left\lfloor\dfrac{a_1n'}{2^{r+3}}\right\rfloor$, we have $k\le\dfrac{a}{n}<k+2$, then $a-nk$ equals either $(a\bmod n)$ or $(a\bmod n)+n$.

For 64-bit processors, under the premise that only two `mul` instructions are performed and no overflow occurs during the reduction, Montgomery reduction can handle every odd number $n<2^{63}$, while this method can handle every $n<2^{62}$ except $n=2^{61}$. Montgomery reduction requires conversion of operands to Montgomery form, while this method requires no conversion.