# skald
> A skald is a term used to describe a poet, especially one from the Viking Age and medieval Scandinavian culture. Skalds were known for their skills in composing and reciting poetry, often in praise of kings, nobles, and other notable figures. Their poetry, known as skaldic poetry, was often complex and featured intricate meter and wordplay. Skalds played an important role in preserving history, legends, and the genealogies of rulers through their oral traditions. Their works were typically composed in Old Norse and have been preserved in various sagas and historical records.


- SISTEMA DE TIPOS (3 TIPOS)
	- `str` (ascii \\0)
	- `real` (float64)
	- `int` (int64)
- ESTRUTURA DO PROGRAMA / SINTAXE
	- `var`/`rav`, `main`/`niam`
    - `loop`/`pool` e `if`/`or`/`fi`, 
	- `stop`, `die`
	- `in`, `out`
	- `.` termina declaração
    - `$` inicia comentário
	- sem chaves, usar `do`
	- sem parênteses no condicional
- OPERAÇÕES
	- atribuição `<-`, ex. `a <- 1`
	- aritmética `+`, `-`, `*`, `/`, `%`, `ˆ`
	- lógica `&`, `|`, `!`
	- relacional `<`, `<=`, `>=`, `>`, `=`, `!=`

Programa exemplo
```
var
	a int.
	b int.
	c int.
	counter int.
rav

main
	a <- 1.
	b <- 1.

	in counter.
	if counter < 0 do
		out "Erro mortal \n".
		die.
	fi
		
	loop counter > 0 do
		out "fibonacci ".
		out a.
        out "\n"

		c <- a + b.
		a <- b.
		b <- c.
		counter <- counter - 1.
		
		if counter = 1 do
			out "Acabando \n".
			stop.
		or
			out "Não acabando \n".
		fi
	pool
niam
```
