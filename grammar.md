
[program] -> [var_block] [main_block]

[var_block] -> var [var_stm_arr] rav
[var_stm_arr] -> [var_stm] [var_stm_arr] | null
[var_stm] -> id type .

[main_block] -> main [main_stm] niam
[main_stm] -> [declar_stm] | [loop_stm] | [if_stm] | [in_stm] | [out_stm] | [stop_stm] | [die_stm] | [comment_stm] | [assign_stm] | [main_stm]

[declar_stm] -. id <- [val] .
[val] -> num | id | str

[loop_stm] -> loop [cond] do [main_stm] pool
[if_stm] -> if [cond] do [main_stm] fi
[cond] -> [val] [rel_op] [val]
[rel_op] -> < | <= | >= | > | = | !=
