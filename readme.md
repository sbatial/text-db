Simple text based (non-permanent) "database" with textual interface.

Born out of a uni excercise, boredom and curiosity.

# Usage

Compile and run with `crun`.

## Available actions

- `:h` to print help message
- `:p` to get current state of db
- `:q` to quit program
- `:x` to delete last message (equivalent: `x <amount of messages>`)
- `:x <num>` to delete message at index `<num>` (does nothing if num > amount of messages)

# Known caveats

I do not have access to a Windows system but to my knowledge Microsoft is unable or unwilling to fully/properly support Unicode which might lead to the table looking scuffed because it uses box-drawing characters as borders (U+2500 - U+257F / U+259F; see <https://en.wikipedia.org/wiki/Box-drawing_character>).
