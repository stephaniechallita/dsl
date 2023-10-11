# Labs on Domain-Specific Languages (DSL)

The lab sessions for this course will consist in realizing the following steps. After each major step, you will need to showcase your work through a small demonstration, which will be used to determine your grade.

## Overall objectives of the lab

These lab sessions will have you realize your very own version of RoboML, a language to help pilot a small four-wheeled robot. Building the DSL will include designing the domain (*i.e.* the concepts and their relationships) as well as associated tooling: this tooling includes the text editor and its services, but also interpretation (through a web-based simulator) and compilation (to Arduino code that will run on the robot itself).

Below, you can find an example of a program that we expect to write in your language.

```
grammar exemple
```

The above program, when executed, should (comportement du programme)

## Part 1 - Domain modeling: definition of the language's domain model with Ecore

The first step in defining a language is to model the concepts and their relationships in the domain targeted by your language. For this part, you can pick any tool you want: the Ecore framework, an online UML editor, or even simply a pen and a piece of paper!

If you chose Ecore, there are some instructions below to help with the technical aspect.

### Ecore modeling

Within your Eclipse RCP that includes EMF, Xtext and Xtend (e.g., the GEMOC Studio), create an _Ecore Modeling Project_. Then you can start modeling your domain as an object-oriented metamodel, which should represent the different concepts of your language and how they are related.

You may validate your metamodel by right click on your ecore model and _Validate_.

When this is done, you can generate the Java-based implementation of your domain model by opening the associated genmodel file, right click on the root element and _Generate all_

You may assess the expressivity of your metamodel (*i.e.*, check if it well captures your domain, in your case meaning it supports the modeling of the proposed example) by opening the ecore metamodel, right clic on the concept of the root element of your expected model, and choose _Create dynamic instance_. Then you can create a model in a tree-based editor, and ensure your metamodel supports the expected model structure. 

## Part 2 - Textual modeling: definition of the Langium grammar and editor for your language

After determining the domain, it is time to move on to the actual text editor for your language. In this lab, we will be building this editor using the TypeScript-based [Langium](https://langium.org/) benchwork to build a Visual Studio Code extension supporting edition of your language.

If not done already, you will need to install a [node environment](https://nodejs.org/en/download) as well as Visual Studio Code, and then run the command `npm i -g yo generator-langium` to install the Langium project generator. Then, run `yo langium` to create the project. This will offer to create a few different things; you can say yes to all of them, pick a language name, extension name and a file extension (*e.g.* .rob).

Depending on what modeling tool you picked in part 1, the next step can change a little bit. If you picked another method than Ecore, skip the following section.

### Ecore modeling

If you decided to model your domain using Ecore and Eclipse in the previous part, you may be aware that it is possible to generate an Xtext project from an Ecore project (by creating a new _Xtext Project From Existing Ecore Models_). Fortunately, it is possible to convert an Xtext grammar into a Langium: simply follow the instructions from the README from [this project](https://github.com/TypeFox/xtext2langium) (be careful, the plugin is not compatible with GEMOC Studio so you will need to use it with another type of Eclipse). You can thus write your grammar using Xtext rather than Langium if you so wish.

The above linked Eclipse plugin will let you convert your Ecore model and Xtext grammar to `.langium` files, which you can put into your `src/language/` folder of the Langium project. Make sure to rename the main grammar file so that it is detected by Langium.

### Other types of modeling

Since you have previously modeled your domain, we need to translate that modeling effort into a meta-model that can be understood by Langium. This can be done by inserting TypeScript-like interfaces in the grammar, the full specification of which can be seen [here](https://langium.org/docs/sematic-model/#declared-types). As an example, this could be the model used for a Finished State Machine language:

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
autre exemple grammaire
```

## Part 3 - Executable modeling

In the previous steps, you have first identified the core concepts of your language, and implemented a textual syntax to define instances of those concepts. The next step is to try and execute those model instances: this can be done either through interpretation or compilation. You will need to implement these as functions in the `generator.ts` file, which you should move to its own `src/generators/` folder.

### Interpretation: 

In this lab, your interpreter will run on a web-based simulator for the robot written in JavaScript. By now, your programs should be parsable, which means Langium will be able to give you an [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) (AST) representing your programs. Interpretation is then just a matter of writing a set of visitor-like functions, which will walk through each node and process the changes to be displayed in the simulator.

(détails techniques de où écrire les fonctions/comment les lier au simulateur)


### Compilation:

Since the objective of this lab is to be able to program a small four-wheeled robot using your language, you will need to be able to compile your code to something the robot can understand - in this case, the robot uses an Arduino card, which can be programmed using a [subset of C](https://www.arduino.cc/reference/en/). You will need to write a compiler that generates Arduino code based on the defined model.

In the same idea as an interpreter, a compiler can also be implemented with functions visiting every node - except instead of directly simulating behaviour, you will generate code that matches the node's semantics.

As previously, you can put your functions in the generator folder. You can then use your compiler by adding a new command to the Command Line Interface provided by Langium, which will be the entry point from which you call the rest of your functions. Registering new commands can be done in `src/cli/main.ts`; once that is done, you should be able to call `./bin/cli compile <source>` in your terminal and have it generate Arduino code corresponding to the source program given as argument.

(détails de comment téléverser sur le robot)