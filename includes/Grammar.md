```txt
command: pipeline conjunctions

conjunctions:/* empty */
            | '&&' pipeline conjunctions
            | '||' pipeline conjunctions

pipeline: command_part pipelines

pipelines: /* empty */
         | '|' command_part pipelines

command_part: built_in_command redirections?
            | redirections? 'command_name' arguments redirections?
            | redirections
            | grouping operators?
            
grouping: '(' command ')'
            
operators: pipelines
         | conjunctions
         | redirections

built_in_command: 'echo' ('-n')? arguments
                | 'cd' 'path'
                | 'pwd'
                | 'export' 'string'
                | 'unset' 'string'
                | 'env'
                | 'exit'

arguments: argument arguments' 
         | /* empty */

arguments': argument arguments'
          | ε

redirections: redirection redirections' 
            | /* empty */
            
redirections': redirection redirections'
             | ε

redirection: '<' 'filename'
           | '>' 'filename'
           | '>>' 'filename'
           | '<<' 'TAG'

argument: quoted_string
        | variable
        | 'string'

quoted_string: '"' string_in_double_quotes '"'
             | "'" string_in_single_quotes "'"

variable: '$' 'string'

string_in_double_quotes: /* matches any sequence of characters, including escaped characters and variable expansion */
string_in_single_quotes: /* matches any sequence of characters, excluding single quotes */

command_name: 'string'
filename: 'string'
path: 'string'
TAG: 'string'
string: /* matches any sequence of non-whitespace characters */
```