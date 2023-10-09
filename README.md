 Simple shell project

Content:

 This program must have the exact same output as sh (/bin/sh) as well as the exact same error output. The only difference is when you print an error, the name of the program must be equivalent to your argv[0].

$ echo "qwerty" | /bin/sh
/bin/sh: 1: qwerty: not found
$ echo "qwerty" | /bin/../bin/sh
/bin/../bin/sh: 1: qwerty: not found
$


$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
$ echo "qwerty" | ./././hsh
./././hsh: 1: qwerty: not found

$Compilation 

Can be compiled with ```commandline $ gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh ```

Colaborators:

> Samuel Ayanwole

> Franca Osawaru
