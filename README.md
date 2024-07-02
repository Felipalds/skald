# skald
> A skald is a term used to describe a poet, especially one from the Viking Age and medieval Scandinavian culture. Skalds were known for their skills in composing and reciting poetry, often in praise of kings, nobles, and other notable figures. Their poetry, known as skaldic poetry, was often complex and featured intricate meter and wordplay. Skalds played an important role in preserving history, legends, and the genealogies of rulers through their oral traditions. Their works were typically composed in Old Norse and have been preserved in various sagas and historical records.


- SISTEMA DE TIPOS (3 TIPOS)
	- string (ascii com \\0)
	- double
	- int64
- ESTRUTURA DE CONTROLE (SELEÇÃO, LAÇOS)
	- var/rav, main/niam, loop/pool e if/else/fi, 
	- stop, die
	- out, in
- SINTAXE
	- sem chaves
	- tem ponto e virgula
	- sem parenteses no if e while
- OPERAÇÕES
	- recebe: <-
	- a <- 1
	- +, -, \*, /, %, ˆ
	- &, |, !
	- <, <=, >=, >, =, !=




```
var
	a int.
	b int.
	c int.
	d int.
rav

main
	a <- 1.
	b <- 1.

	in d.
	if d < 0 do
		out "Erro mortal \n".
		die.
	fi
		
	loop d > 0 do
		out "Variavel a \n".
		out a.
		c <- a + b.
		a <- b.
		b <- c.
		d <- d - 1.
		
		if d = 1 do
			out "Acabando \n".
			stop.
		or
			out "Não acabando \n".
		fi
	pool
niam

```
