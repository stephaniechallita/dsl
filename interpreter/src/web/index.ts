
import { SimulationVisitor } from "../semantics/interpreter.js";
import { Robot } from "../semantics/robot-ml-visitor.js";
import { TypeCheckerVisitor, TypeError } from "../semantics/type-checker.js";
import { Scene, BaseScene } from "./simulator/scene.js"

export function launchSimulation(program: Robot): Scene {
    const scene = new BaseScene();
    program.accept(new SimulationVisitor(scene.robot));
    return scene;
}

export function typecheck(program: Robot): TypeError[] {
    const typechecker = new TypeCheckerVisitor();
    program.accept(typechecker);
    return typechecker.errors;
}