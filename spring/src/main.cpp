#include <factory/scene_factory.h>
#include <factory/render_object_factory.h>
#include <game/game_loop.h>
#include <object/render_object.h>
#include <rendering/renderer.h>
#include <rendering/fbo_rendering/fbo_renderer.h>
#include <vr/spring1D/spring_simulation_1D.h>

#include <spring_gui.h>

#include <memory>
#include <game/factory/game_loop_factory.h>

void InitScene(ifx::GameLoop* game_loop);
void InitSpringGUI(ifx::GameLoop* game_loop,
                   ifx::SpringSimulation1D* simulation);
void InitSpringSimulation(ifx::GameLoop* game_loop,
                          ifx::RenderObject* spring_object,
                          ifx::RenderObject* mass_object);

void InitScene(ifx::GameLoop* game_loop){
    auto scene = game_loop->renderer()->scene();
    scene->camera()->moveTo(glm::vec3(-2.84f, -0.57f, -2.84f));
    scene->camera()->rotateTo(glm::vec3(40.96, 4.81, 40.96));

    auto spring_object = ifx::RenderObjectFactory().CreateSpring();
    auto mass_object = ifx::RenderObjectFactory().CreateMassSpring();
    InitSpringSimulation(game_loop, spring_object.get(), mass_object.get());

/*
    scene->AddRenderObject(std::move(ifx::RenderObjectFactory().CreateRoom()));
*/
    auto floor1 = ifx::RenderObjectFactory().CreateFloor();
    auto floor2 = ifx::RenderObjectFactory().CreateFloor();
    auto floor3 = ifx::RenderObjectFactory().CreateFloor();
    auto floor4 = ifx::RenderObjectFactory().CreateFloor();
    auto floor5 = ifx::RenderObjectFactory().CreateFloor();
    auto floor6 = ifx::RenderObjectFactory().CreateFloor();

    float scaleFactor = 2.0f;
    float a = 2.0f;
    floor1->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor1->rotateTo(glm::vec3(90, 0, 0));
    floor1->moveTo(glm::vec3(0, -a, 0));
    floor2->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor2->rotateTo(glm::vec3(90, 0, 0));
    floor2->moveTo(glm::vec3(0, a, 0));

    floor3->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor3->rotateTo(glm::vec3(0, 0, 0));
    floor3->moveTo(glm::vec3(0, 0.0, -a));

    floor4->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor4->rotateTo(glm::vec3(0, 0, 0));
    floor4->moveTo(glm::vec3(0, 0.0, a));

    floor5->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor5->rotateTo(glm::vec3(0, 90, 0));
    floor5->moveTo(glm::vec3(a, 0.0, 0));

    floor6->scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    floor6->rotateTo(glm::vec3(0, 90, 0));
    floor6->moveTo(glm::vec3(-a, 0.0, 0));

    floor2->SetBeforeRender([](const Program* program){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    });
    floor2->SetAfterRender([](const Program* program){
        glDisable(GL_CULL_FACE);
    });
    floor3->SetBeforeRender([](const Program* program){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    });
    floor3->SetAfterRender([](const Program* program){
        glDisable(GL_CULL_FACE);
    });
    floor4->SetBeforeRender([](const Program* program){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    });
    floor4->SetAfterRender([](const Program* program){
        glDisable(GL_CULL_FACE);
    });
    floor5->SetBeforeRender([](const Program* program){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    });
    floor5->SetAfterRender([](const Program* program){
        glDisable(GL_CULL_FACE);
    });
    floor6->SetBeforeRender([](const Program* program){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    });
    floor6->SetAfterRender([](const Program* program){
        glDisable(GL_CULL_FACE);
    });
    scene->AddRenderObject(std::move(floor1));
    scene->AddRenderObject(std::move(floor2));
    scene->AddRenderObject(std::move(floor3));
    scene->AddRenderObject(std::move(floor4));
    scene->AddRenderObject(std::move(floor5));
    scene->AddRenderObject(std::move(floor6));

    scene->AddRenderObject(std::move(mass_object));
    scene->AddRenderObject(std::move(spring_object));
}

void InitSpringGUI(ifx::GameLoop* game_loop,
                   ifx::SpringSimulation1D* simulation){
    auto gui = std::unique_ptr<ifx::GUI>(
            new SpringGUI(
                    game_loop->renderer()->window()->getHandle(),
                    simulation));
    game_loop->renderer()->SetGUI(std::move(gui));
}

void InitSpringSimulation(ifx::GameLoop* game_loop,
                          ifx::RenderObject* spring_object,
                          ifx::RenderObject* mass_object){
    ifx::Spring1DParameters params;
    params.initial_mass_object.position = 0.0f;
    params.initial_mass_object.velocity = 0.0f;
    params.initial_mass_object.mass = 1.0f;
    params.initial_mass_object.render_object = mass_object;

    params.initial_spring.damping_factor = 1.0f;
    params.initial_spring.spring_factor = 1.0f;
    params.initial_spring.amplitude = 0.00f;
    params.initial_spring.frequency = 0.1f;
    params.initial_spring.phase_shift = 0.1f;
    params.initial_spring.render_object = spring_object;

    params.time_delta = 0.005;

    auto simulation = std::unique_ptr<ifx::SpringSimulation1D>(
            new ifx::SpringSimulation1D(params));
    InitSpringGUI(game_loop, simulation.get());
    game_loop->AddSimulation(std::move(simulation));
}

int main() {
    auto game_loop = ifx::GameLoopFactory().Create();
    InitScene(game_loop.get());

    game_loop->Start();
}

