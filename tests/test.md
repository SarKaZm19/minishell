[minishell correction sheet](https://rphlr.github.io/42-Evals/Cursus/Minishell/)

# Tester list
- https://github.com/zstenger93/42_minishell_tester
  - looks great but ask some config
- https://github.com/thallard/minishell_tester
  - test command with `[]`
- https://github.com/ChewyToast/mpanic
	looks great, just didn't understand the required config
- https://github.com/mcombeau/minitester-minishell-tester
	test with `;`
- https://github.com/simon-ameye/42-minishell_bash_tester_2022
- https://github.com/LucasKuhn/minishell_tester
- https://github.com/we-sh/42ShellTester
	complete, very slow, test with `;`

# Random test (or not)
- [`bash | ls`, `cat | ls`](https://42born2code.slack.com/archives/CMX2R5JSW/p1685642637068909) --> A faire
- `ls && > out`   --> No error raised in the lexer tokens are : ls, &&, >, out
- `ls | > out` --> No error raised in the lexer tokens are : ls, |, >, out
- (maybe not not to manage: ) `ls |` --> No error raised in the lexer tokens are : ls, | (i saw people just returning the prompt if the command starts or finishes by a pipe)
- about `<>`: https://42born2code.slack.com/archives/CN9RHKQHW/p1701778738022009
- Run Minishell in Minishell in Minishell and see how you handle signals
- do a bunch of heredocs
- See how many pipes it can handle like "ls | ls | ls | ls | ls..." until it crashes (it should be able to handle >100 easily)
- https://42born2code.slack.com/archives/CMX2R5JSW/p1703574479836979

# Parser
- Logic order `echo 1 || echo 2 && echo 3` vs `echo 1 || (echo 2 && echo 3)`
- Simple test but complete (no redirection and no wildcards): `((ls -Rla | wc -c) || (pwd | cat)) && ((cat philosophers/philo/Makefile | wc -l) || (echo -n "Hello World"))`
- A little bit everything: `(cat file1.txt | grep 'pattern' > output.txt) && echo "Search complete" || (echo "Error occurred" > error.log)`
- `( hi ( ( hello ) common ) )`
- check redirection on ls, not all the pipe `> out ls | grep l > out 2 | echo hello`

# Redirection
- A simple example is that in Bash echo `foo > a > b` will create both files `a` and `b`, but a will be empty while b will contain the string foo.Note that if you run this in zsh, both a and b will contain the string foo! Always double-check that you're actually in Bash. (https://42born2code.slack.com/archives/CN9RHKQHW/p1678124734122869?thread_ts=1678121496.374609&cid=CN9RHKQHW)
- `cat < file1 >> file2 -e file3 << file4 << file5 file6`  --> Test with Lexer :
	Tokens are : cat, <, file1, >>, file2, -e, file3, <<, file4, <<, file5, file6
	tested with : cat < Makefile >> file2 -e tokens.json << file4 << file5 ast.json  so it works with normal shell 
	ask for heredoc until file4 (EOF word)
	ask for heredoc until file5 (EOF word)
	then print tokens.json + ast.json in file2
	(tokens look right with that test too)
- `<< EOF > out1`
- `<< EOF > out1 > out2`
- `out1 >> END`
- `ls | > out grep l | cat`

# Kinda """Complete""" tests done on the lexer that are correct for now
grep -r "pattern" /var/log | sort | uniq > result.txt
cat file1.txt file2.txt | grep "keyword" | sed 's/old/new/g' > output.txt
ls -l | awk '{print $9}' | xargs rm
ps aux | grep "process" | awk '{print $2}' | xargs kill -9
tar -czf archive.tar.gz --exclude=*.log /var/www
find /home/user -name "*.txt" -exec cp {} /backup \;
echo "Hello, World!" | tee output.txt | grep "Hello" | wc -l
cat file.txt | grep "error" | sed 's/error/ERROR/' > corrected_file.txt
find / -type f -name "*.log" -exec rm {} \;
ls -l | awk '$1 ~ /^d/ {print $9}' | xargs du -sh
echo "First line" > file.txt ; echo "Second line" >> file.txt
grep -E "pattern1|pattern2" input.txt | sed 's/pattern/replace/' | sort > output.txt
echo "Line 1\nLine 2\nLine 3" | awk '{print length, $0}' | sort -n | cut -d" " -f2-
ps aux | awk '{print $1}' | sort | uniq | xargs -I{} sh -c 'echo "User: {}"; ls /home/{}'
find / -type f -name "*.txt" -exec grep -H "search_term" {} \; > search_results.txt
ls -l | grep "^d" | awk '{print $NF}' | xargs -I{} sh -c 'echo "Files in {}: "; ls {}'
grep -r "error" /var/log | awk -F: '{print $1, $2}' | sort | uniq > error_summary.txt
ps aux | grep "process" | awk '{print $2, $11}' | sort -k2 | uniq > process_list.txt
find / -type f -name "*.log" -exec cat {} \; | grep "error" | wc -l
grep -rl "pattern" /var/www | xargs sed -i 's/pattern/new_pattern/g'
