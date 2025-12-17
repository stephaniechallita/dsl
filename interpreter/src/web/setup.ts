import { MonacoLanguageClient } from 'monaco-languageclient';
import { Scene } from './simulator/scene.js';
import { Wall } from './lib/wall.js';
import p5 from "./lib/sketch.js";
import { Robot } from './lib/robot.js';

export function setup(client: MonacoLanguageClient, uri: string) {

    const win = window as any;

    const typecheck = (() => {
        console.info('typechecking current code...');
        client.sendNotification('typecheck', uri);
        client.onNotification('typecheck', (errors: { msg: string, code: string }[]) => {
            let content = document.querySelector("#errorModal .modal-body")!;
            content.innerHTML = ""
            console.log(errors)
            errors.forEach(error => {
                let errorText = "<p><span class='error'>RoboML Error: " + error.msg + "</span>\n<br>\n";
                if (error.code) {
                    const parts = error.code.split("\n");
                    errorText += parts[0];
                }
                content.innerHTML += errorText + "</p>\n";
            })
            if (errors.length > 0) {
                const modal = document.getElementById("errorModal")!;
                modal.style.display = "block";
            } else {
                const modal = document.getElementById("validModal")!;
                modal.style.display = "block";
            }
        });
    });

    const execute = (async () => {
        console.info('running current code...');
        client.sendNotification('execute', uri);
        client.onNotification('execute', async (scene) => {
            console.log(scene);
            setupSimulator(scene);
        });
    });

    const setupSimulator = (scene: Scene) => {
        const wideSide = Math.max(scene.size.x, scene.size.y);
        let factor = 1000 / wideSide;

        win.scene = scene;

        scene.entities.forEach((entity) => {
            if (entity.type === "Wall") {
                win.entities.push(new Wall(
                    (entity.pos.x) * factor,
                    (entity.pos.y) * factor,
                    (entity.size.x) * factor,
                    (entity.size.y) * factor,
                    p5
                ));
            }
            if (entity.type === "Block") {
                win.entities.push(new Wall(
                    (entity.pos.x) * factor,
                    (entity.pos.y) * factor,
                    (entity.size.x) * factor,
                    (entity.size.y) * factor,
                    p5
                ));
            }
        });

        win.p5robot = new Robot(
            factor,
            scene.robot.pos.x,
            scene.robot.pos.y,
            scene.robot.size.x * factor,
            scene.robot.size.y * factor,
            scene.robot.rad,
            p5
        );
    }

    win.execute = execute;
    win.parseAndValidate = typecheck;

    var errorModal = document.getElementById("errorModal")!;
    var validModal = document.getElementById("validModal")!;
    var closeError = document.querySelector("#errorModal .close") as HTMLElement;
    var closeValid = document.querySelector("#validModal .close") as HTMLElement;
    
    closeError.onclick = function () {
        errorModal.style.display = "none";
    }
    closeValid.onclick = function () {
        validModal.style.display = "none";
    }
    window.onclick = function (event) {
        if (event.target == validModal) {
            validModal.style.display = "none";
        }
        if (event.target == errorModal) {
            errorModal.style.display = "none";
        }
    }
}