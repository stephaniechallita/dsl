# Labs on Domain-Specific Languages (DSL)



These lab sessions are to be realized in groups of two.

## Overall objectives of the lab

During these lab sessions, you will create your own version of RoboML, a language to define the behavior of a small robot. Building the DSL will include modeling the domain (*i.e.* the concepts and their relationships), and implements the associated tooling: *e.g.* the text editor and its services, an interpreter (through a web-based simulator) and a compiler (to Arduino code that will run on the robot itself).

Below, you can find an example of a program that we expect to write in your language.

```
let void entry () {
    setSpeed(150 in mm) // distance per second (here 150mm/s)
    var number count = 0
    loop count < 5
    {	
        count = count + 1
        square()
    }
}

let void square(){
    Forward 30 in cm
    Clock 90
    Forward 300 in mm
    Clock 90
    Forward 30 in cm
    Clock 90
    Forward 300 in mm
    Clock 90
}
```

The above program, when executed, should set the speed of the robot and then perform 5 time (loop) a square pattern movement (square function).

The robot used for this lab has four wheels with individual motors, and an ultrasound sensor which can be used to measure distance to an object in front. We expect your language to mostly follow the imperative programming paradigm, with basic arithmetic and commands for the robot.

## Evaluation

These labs are split in three parts corresponding to the three main aspects of DSLs implementation: abstract syntax, concrete syntax and semantics.

The lab sessions for this course will consist in the realization of these aspects. After each major step, you will need to showcase your work to your lab teacher through a small demonstration, which will be used to grade you. You can only move onto the next step once the teacher has validated the current part.

A final evaluation of your work will be done at the end, based on your code.
This code should be pushed to a GitLab/GitHub repository, and the link sent to your lab teacher.

To encourage you to finish completely at least one of the two possible semantics, yet exploring both compilation and interpretation, you will have to choose a major and a minor semantics.
This choice should be clearly stated in the README file of the git repository.

The evaluation grid is the following:
- Abstract syntax : 5
- Concrete Syntax : 5
- Semantics : 10
    * Major : 7
    * Minor : 3

The **deadline** for the project is the ___26/05/2024 at 23:59___.
Commits after this date will be ignored.

## Part 1 - Domain modeling: definition of the language's metamodel with Ecore

The first step in defining a language is to model the concepts and their relationships in the domain targeted by your language. 
This result in the creation of the language's metamodel, defining its abstract syntax.
For this part, you can pick any tool you want: the Ecore framework, an online UML editor, or even simply a pen and a piece of paper!

If you chose Ecore, there are some instructions below to help with the technical aspect.

There is a short list of mandatory concepts that we want:
- Movement (front, back, sides)
- Rotation
- Speed
- Sensors (time, distance in front of the robot)
- Units (*e.g.,* cm, mm)
- Basic arithmetic and boolean expressions
- Control structure (loop, conditions)
- Functions and variables

**N.B. :** For units, you can either implement it as "cast function" or as a concrete type in the language.
> For units, you can either implement it as "cast function" or as a concrete type in the language.  
As a cast -> `var number length = 10 in cm`.  
As a type -> `var cm length = 10`.

### Ecore modeling

Within your Eclipse RCP that includes EMF, Xtext and Xtend (e.g., [Eclipse DSL](https://www.eclipse.org/downloads/packages/release/2023-09/r/eclipse-ide-java-developers) with the added [Ecore tools](https://projects.eclipse.org/projects/modeling.emft.ecoretools) which you can install with _Help_ -> _Eclipse Marketplace..._ -> search for `ecoretools`), create an _Ecore Modeling Project_. Then you can start modeling your domain as an object-oriented metamodel, which should represent the different concepts of your language and how they are related.

You may validate your metamodel by right-clicking on your ecore model and then clicking on _Validate_.

When this is done, you can generate the Java-based implementation of your domain model by opening the associated `genmodel` file, right-clicking on the root element and _Generate all_

You may assess the expressivity of your metamodel (*i.e.*, check if it captures your domain well, in your case meaning it supports the modeling of the proposed example) by opening the Ecore metamodel, right-click on the concept of the root element of your expected model, and choose _Create dynamic instance_. Then you can create a model in a tree-based editor, and ensure your metamodel supports the expected model structure. 

## Part 2 - Textual modeling: definition of the Langium grammar and editor for your language

After determining the domain, it is time to move on to the actual text editor for your language. In this lab, we will be building this editor using the TypeScript-based [Langium](https://langium.org/) workbench to build a Visual Studio Code extension supporting edition of your language.

If not done already, you will need to install a [node environment](https://nodejs.org/en/download) as well as [Visual Studio Code](https://code.visualstudio.com/docs/setup/setup-overview), and then run the command `npm i -g yo@4.3.1 generator-langium@2.0.0` to install the Langium project generator. Then, run `yo langium` to create the project. This will offer to create a few different things; you **have to** say yes to all of them, pick a language name, extension name and a file extension (*e.g.* .rob).

> We use particular version of yo and generator-langium in these labs due to the rapid change in version of langium.
**Make sure that you use these versions**.

Depending on what modeling tool you picked in part 1, the next step can change a little bit. If you picked another method than Ecore, skip the following section.

**N.B.**: There is flexibility on the concrete syntax of the language, but make it concise and usable for non-expert in programming. Ask your teacher during labs if you plan to change the syntax.

### Ecore modeling

If you decided to model your domain using Ecore and Eclipse in the previous part, you may be aware that it is possible to generate an Xtext project from an Ecore project (by creating a new _Xtext Project From Existing Ecore Models_). Fortunately, it is possible to convert an Xtext grammar into a Langium: simply follow the instructions from the README of [this project](https://github.com/TypeFox/xtext2langium). You can thus write your grammar using Xtext rather than Langium if you so wish.

The above linked Eclipse plugin will let you convert your Ecore model and Xtext grammar to `.langium` files, which you can put into your `src/language/` folder of the Langium project. Make sure the grammars names match up between your projects, otherwise you will have to manually refactor the conflicts.

### Other types of modeling

Since you have previously modeled your domain, we need to translate that modeling effort into a metamodel that can be understood by Langium. This can be done by inserting TypeScript-like interfaces in the grammar, the full specification of which can be seen [here](https://langium.org/docs/reference/semantic-model/#declared-types). As an example, this could be the model used for a Finished State Machine language:

```ts
interface StateMachine {
    name: string
    states: State[]
    transitions: Transition[]
    initialState: State
}

interface State {
    name: string
    output: string
}

interface Transition {
    name: string
    input: string
    start: @State
    target: @State
}
```

You can then write the textual grammar rules that will return your specified types.

### VSCode extension with Langium

Once you have a valid [Langium grammar](https://langium.org/docs/grammar-language/), you can launch the commands `npm run langium:generate` and `npm run build` to build the project.

You can test your editor as you make changes either by launching the command `code --extensionDevelopmentPath=$PWD`, or by starting a debug session in VSCode, both of which will open a VSCode instance with your extension loaded in. You can create example and test files for your language in this instance. A goal for this part can be to have your language parse the examples given in this document, such as the following:

```
let void entry () {
    setSpeed(200 mm) // distance per second (here 200mm/s)
    var number time = getTimestamp()
    loop time < 60000
    {
        var number dist = getDistance() in cm
        Forward dist - 25 in cm
        Clock 90
        time = getTimestamp()
    }
}
```

**N.B.**: If you have made changes on the syntax, the equivalent AST with your syntax is your test.

## Part 3 - Executable modeling

In the previous steps, you have first identified the core concepts of your language, and implemented a textual syntax to define instances of those concepts. 
By now, your programs should be parsable, which means Langium will be able to give you an [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) (AST) representing your programs. 
The next step is to try and execute those model instances: this can be done either through interpretation or compilation. You will need to implement these in a `compiler.ts` and `interpreter.ts` file (take a look at the `generator.ts` in the cli folder), which you should put in a new `src/semantics/` folder.

To execute the program, you will use the [visitor design pattern](https://en.wikipedia.org/wiki/Visitor_pattern) to implement a compiler targeting the Arduino language (allowing the execution on a real robot) and an interpreter directly executing the program.
The visitor pattern allows to split the language definition in two parts, the syntax (abstract syntax defined through the metamodel and concrete syntax defined by the grammar) and the semantics (interpreter and compiler), easing the extension/evolution of the language semantics.
Each method implemented in a visitor represents the semantics of a concept, often relying on the semantics of its child in the AST.

You will find in the `VisitorPattern` folder a template code to define the visitor interface, and the accept weaver to add the accept method to the node of the AST.
Further explanation are detailed in the comments of the provided files.

> In addition to the template, a python generator is provided.
This generator take as parameter a file containing the concepts names (1 per line), and the project name (`"projectName"` in `langium-config.json`)

### Interpretation: 

In this lab, your interpreter will run on a web-based simulator for the robot written in JavaScript.
You will find in the interpreter folder of this repository, the code of the simulator provided for this part of the lab.
In the `interpreter.ts`, implement the visitor

You will find in the `Interpreter` folder the code of the simulator.
The Typescript files in the `web/simulator` folder represent the elements of the simulation used in your interpreter.
Especially, you will find the *Robot* class that will be manipulated by your interpreter.
In addition, you will find the scene classes representing the environment in which the robot evolves.  
The scene **REQUIRES** you to add timestamps objects recording the steps of the simulation to replay it in the web page.  
The JavaScript files in the `static/simulator` folder are used to display the simulation on the web page.
This JavaScript code expects to receive the final state of the scene simulated.

To understand how to create the communication between the LSP server and client, we propose you to first create a 'parseAndValidate' LSP action.
The general idea of the 'parseAndValidate' action can be found [here](https://web.archive.org/web/20230323045804/https://langium.org/tutorials/customizing_cli/), while the code required to define new LSP action usable in the web is detailed [here](https://web.archive.org/web/20230323041439/https://langium.org/tutorials/generation_in_the_web/)

**N.B.** the `setup.js` file already contains parts of the required code

### Compilation:

Since the objective of this lab is to be able to program a small four-wheeled robot using your language, you will need to be able to compile your code to something the robot can understand - in this case, the robot uses an Arduino card, which can be programmed using a [subset of C](https://www.arduino.cc/reference/en/). You will need to write a compiler that generates Arduino code based on the defined model.

To test your compiler, you will need the [Arduino IDE](https://www.arduino.cc/en/software). At first, you will not need the robot and just verify that the generated code is valid (Verify action in Arduino IDE).
When your generator generates valid Arduino programs, ask your teacher the robot to verify the correct (or not) behavior.

In the same idea as an interpreter, a compiler can also be implemented using a visitor pattern - but instead of directly simulating the behavior, you will generate the Arduino code representing this behavior.

As previously, you can put your visitor in the semantics folder. You can then use your compiler by adding a new command to the Command Line Interface provided by Langium, which will be the entry point from which you call the rest of your functions. Registering new commands can be done in `src/cli/main.ts`; once that is done, you should be able to call `./bin/cli.js compile <source>` in your terminal and have it generate Arduino code corresponding to the source program given as argument.

To understand how to call the semantics from the command line, we propose you to first create a 'parseAndValidate' action.
The description of the 'parseAndValidate' action can be found [here](https://web.archive.org/web/20230323045804/https://langium.org/tutorials/customizing_cli/).
After that you will be able to call your visitor in a 'generate' action.

You will find in the `Compiler` folder an example of code to control the robot.
The global structure of this program will not require many changes.
If you want details on the possible actions, go look at the definition of the `demoAction` function used in the example, it uses most of the possible movements.
> You can find it in the MotorWheel lib, in the `Omni4WD.cpp` file

**WARNING :** This robot requires non-classical libraries, you will have to add them.
Copy the folders in the `compiler/Arduino Example/lib/` folder in the `libraries` folder of your Arduino IDE.




