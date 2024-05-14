import re
import argparse
from string import Template

parser = argparse.ArgumentParser()
parser.add_argument("file", help="File listing language concepts (1 per line)")
parser.add_argument("project", help="Name of the Langium project")
parser.add_argument("-o", "--out", type=str, default=".",
                    help="path to output folder")
args = parser.parse_args()

pattern = re.compile(r'(?<!^)(?=[A-Z])')
def camelToSnake(string):
    return pattern.sub('-', string).lower()

weaveFunctionBuilder = Template('''
weave$concept(node : InterfaceAST.$concept, accept : ValidationAcceptor) : void {
    (<any> node).accept = (visitor: Visitor) => { return visitor.visit$concept(node as unknown as ClassAST.$concept); }
}
''')

weaveChecksBuilder = Template("$concept : this.weave$concept")

visitorClassesBuilder = Template('''
export class $concept implements ASTInterfaces.$concept {
    // the constructor must take all attribute of the implemented interface 
    // simply copy-paste the interface fields as public parameters
    // you can find them in generated/ast.ts
    constructor(public $$type: '${concept}'){}
    accept(visitor: Visitor) : any {}
}
''')

visitorSignatureBuilder = Template("visit$concept(node : $concept) : any;")



weaveFunctions = []
weaveChecks = []
visitorClasses = []
visitorSignatures = []



conceptList = open(args.file, 'r')
concepts = conceptList.readlines()
 
for concept in concepts:
    conceptName = concept.strip()
    weaveFunctions.append(weaveFunctionBuilder.substitute(concept=conceptName))
    weaveChecks.append(weaveChecksBuilder.substitute(concept=conceptName))
    visitorClasses.append(visitorClassesBuilder.substitute(concept=conceptName))
    visitorSignatures.append(visitorSignatureBuilder.substitute(concept=conceptName))


visitor = open(args.out + "/visitor.ts", "w")
visitorFile = Template('''
import * as ASTInterfaces from '../generated/ast.js';
import { Reference } from 'langium';

export interface Visitor{
    $signatures
}

$classes
''')
visitor.write(visitorFile.substitute(
    signatures="\n\t".join(visitorSignatures), 
    classes="".join(visitorClasses),
    project=args.project)
    )
visitor.close()

acceptWeaver = open(args.out + "/accept-weaver.ts", "w")
acceptWeaverFile = Template('''
import type { ValidationAcceptor, ValidationChecks } from 'langium';
import type { ${project}AstType } from '../generated/ast.js';
import * as InterfaceAST from '../generated/ast.js';
import * as ClassAST from './visitor.js';
import { Visitor } from './visitor.js';
import type { ${project}Services } from '../${projectSnake}-module.js';

/**
 * Register custom validation checks.
 * TODO : Call this function in the language module.ts file (see registerValidationChecks(...);)
 */
export function weaveAcceptMethods(services: ${project}Services) {
    const registry = services.validation.ValidationRegistry;
    const weaver = services.validation.${project}AcceptWeaver
    registry.register(weaver.checks, weaver);
}

export class ${project}AcceptWeaver {
    
    // TODO : Remove lines for abstract concepts
    checks: ValidationChecks<${project}AstType> = {
        $checks
    };

    $functions

}
''')



acceptWeaver.write(acceptWeaverFile.substitute(
    checks=",\n\t\t".join(weaveChecks), 
    functions="".join(weaveFunctions),
    project=args.project,
    projectSnake=camelToSnake(args.project))
    )
acceptWeaver.close()