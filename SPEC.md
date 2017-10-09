# Shakara Specification

Shakara is a dynamically typed toy scripting
language created for fun.

Use in your own projects at your own risk!
Due to this being a toy project, it may
be super insecure and prone to bugs.

For features that may, or may not be, fleshed out,
their titles will be *italicized*. Subsequently when
they are implemented, they will be unitalicized.

### Important Constructs

With Shakara being dynamically typed, the programmer
does not deal with types, at all. Not once.
Of course, should be **mindful** of what types you are
using, but you can't explicitly set types.

Additionally, there are no semicolons, or any type of
terminator for statements in Shakara, just finish a
statement and make a newline, or a space, and volia,
new statement.

### Integers

Integers are by default 32-bit, and are specified with just
a single number without a decimal point anywhere.

### Decimals

Decimals in Shakara are a floating point number, defined by
specifying any number with a decimal point.

### Strings

Strings in Shakara are specified by wrapping characters in two
double quotes.

The amount of characters in a string can be grabbed with the
`amt` function, passing the string in as an argument.

You may access a character in a string using an array access.

##### Escape Sequences

Shakara strings, like many other languages, support escape sequences
to use characters that could be seen by the parser, or otherwise would
be parsed.

The ones supported are in the table below.

| Sequence  |  Result      |
| --------- | ------------ |
| \\n       | newline      |
| \\\       | backslash    |
| \\"       | double quote |

### Booleans

Booleans in Shakara are simple `true` or `false` keywords, implemented
as a `bool` in the underlying C++ interpreter.

### Arithmetic

Shakara does arithmetic expressions in the most
straightforward way possible.
This way is through parsing each individual portion
of a expression in order.

This means, if you type `1 * 6 * 7 / 6 * 9`,
you will get `63`. I may add operator precedence
at some point, or order expressions like an elementary
math expression, but not as of now.

Also supported are the operators as follows: `+=`, `-=`,
`*=`, `/=`, `++`, and `--`. Which function the same as
languages like C or C++.

### Comparisons

Shakara also supports comparisons like in C-like languages.
The operations for comparisons are `==`, `!=`, `<`, `>`, `<=`,
and `>=`.

Booleans do not correspond with integers as well, so `1` and `true`
are not interchangable, nor is `0` and `false`. If a value should
be used as a boolean, use an actual boolean.

Each of these comparisons return boolean values if used in definitons
or function arguments.

Comparisons must also be done on compatible types. Thus, you cannot
say `1 == "1"` or `1 == true`.

### Variables

Since Shakara is a dynamically typed language, you don't
have to specify a single type when defining a variable.
In fact, you cannot specify types, simplifying writing
code significantly.

Defining a variable uses this very simple syntax: `<variable_name> = (value)`. That's all.

### Functions

Defining functions in Shakara follows the same rough
syntax of defining variables, with some syntax akin
to say, C/C++.

Below is the syntax for function definition:

    <function_name> = (<arguments>)
    {
        (body)
    }

Functions are defined using the equals sign and then the arguments
as a way to mirror assignment of actual variables (Also, I felt it
was a little more straightforward to parse at that point).

Then, after being defined, to call a function, use the line
below:

    <function_name>(<args>)

### Printing

Printing is a built in call in the language, no imports required
or anything of the sort.

Print statements follow the style of a function call, usage would
be like the code below:

    print(type, other_type)

Print supports as many arguments as you need, and these arguments
will either be printed by their value, or if their value cannot be
determined, it will print the current type of the variable.

Print does not automatically create a new line at the end of the
method. Thus, to add a new line, you can do the following:

    print(types, "\n")

Or, you can embed a new line character, `\n`, in the string that you
are printing.

### Return

Returning can be used within a method to, of course, return some
kind of value.

Return is roughly the same as languages like C/C++ or even Python.
The code for using a return is like below

    counter = (count)
    {
        return count + 1
    }

Return cannot be used outside of a function.

### If Statements

If statements are like any other C-like language, with the following syntax:

    if (condition)
    {
        block
    }

Also, like many other C-like languages, braces in if statements can be omitted, but then only one statement is allowed in the body.

    if (condition)
        statement

You can also use `else` or `else if` like any other C-like language.

### While Loops

Like other C-like languages, you can use a while loop with the following
syntax:

    while (condition)
    {
        block
    }

Like with the if statements, you may omit braces if a single statement
is to be used in the loop.

### Arrays

Arrays are a necessary type for any programming language, what's currently
in the air though is the syntax and form of these arrays. My thoughts are
as follows.

The syntax for arrays could look something like the following for definition:

    array_name = [size] { elements }

Which would create a fixed array with the following elements within.

Elements would be specified with a comma between each separate element:

    { element1, element2, element3, ... }

Though, for instance, the elements portion can be omitted to look like this:

    array_name = [size]

Which creates a fixed size array that is empty.

Though the reverse in my mind will be possible, the size could be omitted,
either with or without the elements, to look like this:

    array_name = [] { elements }

For any of the types, the brackets would be required, but the above creates
a dynamic array, with the elements specified, if specified.

This may be odd for a dynamic language, to have a fixed size array, but I
feel that this could be beneficial in the long run. For instance, there are
many cases that you would want to enforce a fixed-size array, and in these
instances, in other languages, you would ensure that the size does not exceed
your maximum size in your own code. With a fixed size array, the interpreter
could worry about this for you, cutting down on code necessary for this.
In addition, fixed size arrays are helpful for overall memory usage, as you
don't have to grow the array to accommodate for extra elements.

Grabbing the amount of items in the array would be used as a function, like in Python:

    amt(array_name)

Arrays in Shakara are to be zero-based, and would be accessed like most C-like languages, as shown below:

    array_name[index]

For arrays, you can insert and remove from them using the `push` and `pop` methods respectively

    push(array, <element>)

Pop is different in that it takes the array and then an index as to where the element to remove is

    pop(array, <element index>)

### Command Line Arguments

In many programs, you will need to, at some point, access the command line
arguments that were passed to the interpreter. These parameters are exposed
as an array, and can be accessed with the identifier `shk__cmd_args` (the `shk` prefix is used so that others can use the name `cmd_args`, I may go
back on this though).

Since it's an array, you can use `amt(shk__cmd_args)` to grab the amount
of arguments.

The arguments array's first element will always start with the path of the
Shakara file ran, and then go into the subsequent arguments.

### Exiting

If you wish to exit a script prematurely, use the `exit` function. Like below:

    exit()

You may also add an optional integer exit code, like such:

    exit(1)

### *For-each Loops*

In attempt to make iterating through collections simple, implementing a
syntax like D could be beneficial. In my head this is how you could start
it:

    foreach (collection)

This will iterate through the collection, and create a variable called
`item` which will be the item that you are currently on. You may also
want to rename this `item` variable, which you can do like below:

    foreach (collection, thing)

Which will then set the current item to the identifier `thing` rather
than `item`. You may also want to get the current index, which you can
do like this:

    foreach (collection, thing, index)

This will also set a variable called `index` which will, obviously,
contain the index of the loop.

You may also get just the index and not reset the `item` variable
name, like below:

    foreach (collection,, index)

### *Structs*

For Shakara, I want to experiment with only using struct types as containers
with no real inheritance to speak of. I may go back on this at some point
but this is the direction I'd like to go right now.

Structs would reflect their C-like counter parts, being defined with the
syntax below:

    my_struct
    {
        name = ""
        
        set_name = (new_name)
        {
            name = new_name
        }

        get_name = ()
        {
            return name
        }
    }

### *Input-Output Operators*

When programming, a common pattern is I/O. Take an Image for example.
When creating an Image for modification in a language, you first load
an image from a file, or create from an array (or similar) of pixels.

This common pattern can be multiple lines usually, or even more,
depending on the API. But, with the idea of input-output operators,
this example can be shortened. Take this code for example

    image <- "path/to/image/file"

    // do image operations

    // imagine that the screen variable
    // is something such as a window in
    // which to draw to
    image -> screen

This could also be extended to file loading, by creating a reusable
instance of a file loader, you could theoretically do something like
the following example:

    loader = FileLoader()
    loader.readLineByLine(true)

    buffer = ""

    loader <- "path/to/file"

    while (loader.hasFile())
        loader -> buffer

This could be extended to many more applications to simplify code for
input and output significantly.

C++, for instance, has this sort of construct, with the `<<` and `>>`, but this is more hacky than anything, as it piggybacks off of the bitshift operators. With a dedicated operator, there could be more possibilty for a feature such as this.