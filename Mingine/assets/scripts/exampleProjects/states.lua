dofile(scriptDirectory .. "core/stateMachine.lua")
dofile(scriptDirectory .. "core/steering.lua")

SCREEN_WIDTH = 1024
SCREEN_HEIGHT = 768

MAX_ACCELERATION = 0.5 -- how fast can the agent change direction and speed?

CHASE_START_DISTANCE = 200
CHASE_END_DISTANCE = 250

gameState = 0

timer = 0
timeLeft = 15000

--------------------------------------------
-- agent state behaviors
--------------------------------------------
function GetDistanceToMouse(agent)
    mouseX, mouseY = GetMousePosition()
    toMouse = {}
    toMouse.x, toMouse.y = VectorTo(agent.x, agent.y, mouseX, mouseY)
    
    return Magnitude(toMouse.x, toMouse.y)
end

--------------------------------------------
-- agent state behaviors
--------------------------------------------

function WanderEnter(agent)
    agent.r = 255
    agent.g = 255
    agent.b = 255
    
    MAX_ACCELERATION = 0.5
end

function WanderUpdate(agent)
    local x, y = Wander(agent)
    x, y = Normalize(x, y)
    agent.acceleration.x, agent.acceleration.y = Scale(x, y, MAX_ACCELERATION)
        
    TurnTo(agent, agent.velocity)
    UpdateEntity(agent)
    
    -- confine agent to visible screen
    if agent.x > SCREEN_WIDTH then agent.x = 0 end
    if agent.y > SCREEN_HEIGHT then agent.y = 0 end
    if agent.x < 0 then agent.x = SCREEN_WIDTH end
    if agent.y < 0 then agent.y = SCREEN_HEIGHT end
    
    --transitions
    if GetDistanceToMouse(agent) < CHASE_START_DISTANCE then 
        EnterState(agent, chaseState)
    end
end

function WanderExit(agent)
    PlaySound(alertSfx)
end

function ChaseEnter(agent)
    agent.r = 255
    agent.g = 0
    agent.b = 0
    
    MAX_ACCELERATION = 0.8
end

function ChaseUpdate(agent)
    mouseX, mouseY = GetMousePosition()
    local x, y = Seek(agent, mouseX, mouseY)
    x, y = Normalize(x, y)
    agent.acceleration.x, agent.acceleration.y = Scale(x, y, MAX_ACCELERATION)
       
    TurnTo(agent, agent.velocity)
    UpdateEntity(agent)
    
    --transitions
    if GetDistanceToMouse(agent) > CHASE_END_DISTANCE then 
        EnterState(agent, wanderState)
    elseif GetDistanceToMouse(agent) < 10 then
        gameState = -1;
    end
end

function ChaseExit(agent)
    --do nothing
end



function PacingEnter(agent)
    if not agent.dirX then
        agent.dirX = 1
    end

    agent.startX = agent.x
end

function PacingUpdate(agent)
    agent.velocity.y = 0

    agent.velocity.x = 5 * agent.dirX
    UpdateEntity(agent)

    local yDir = 1;
    if agent.y > SCREEN_HEIGHT/2 then
        yDir = -1
    end

    TurnTo(agent, CreateVector(0, yDir))

    if agent.x < 0 or agent.x > SCREEN_WIDTH then
        agent.dirX = agent.dirX * -1;
    end

    if math.abs(agent.x - agent.startX) > 100 then
        EnterState(agent, passState)
    end

    if GetDistanceToMouse(agent) < 32 then
        gameState = -1;
    end
end

function PacingExit(agent)

end


function PassEnter(agent)
    agent.velocity.x = 0

    agent.readyToSwitch = false
    agent.dirY = 1
    if agent.y > SCREEN_HEIGHT/2 then
        agent.dirY = -1;
    end
end

function PassUpdate(agent)
    agent.velocity.y = 20 * agent.dirY
    UpdateEntity(agent)

    local yDir = 1;
    if agent.y > SCREEN_HEIGHT/2 then
        yDir = -1
    end

    TurnTo(agent, CreateVector(0, yDir))

    if agent.y >= SCREEN_HEIGHT-32 or agent.y <= 32 then
        agent.dirY = agent.dirY * -1

        EnterState(agent, pacingState)
    end

    if GetDistanceToMouse(agent) < 32 then
        gameState = -1;
    end
end

function PassExit(agent)

end

--------------------------------------------
-- state objects
--------------------------------------------

wanderState = {}
wanderState.name = "Wander"
wanderState.Enter = WanderEnter
wanderState.Update = WanderUpdate
wanderState.Exit = WanderExit

chaseState = {}
chaseState.name = "Chase"
chaseState.Enter = ChaseEnter
chaseState.Update = ChaseUpdate
chaseState.Exit = ChaseExit

pacingState = {}
pacingState.name = "Pacing"
pacingState.Enter = PacingEnter
pacingState.Update = PacingUpdate
pacingState.Exit = PacingExit

passState = {}
passState.name = "Pass"
passState.Enter = PassEnter
passState.Update = PassUpdate
passState.Exit = PassExit
--------------------------------------------
-- mingine hooks
--------------------------------------------

function Start()
    CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT)
    SetWindowTitle("Agent State Machine Example.")
    
    font = LoadFont("fonts/8_bit_pusab.ttf", 16)
    alertSfx = LoadSound("sfx/happy.wav")
    
    local image = LoadImage("images/arrow.png")

    agents = {}

    for i=1, 10 do 
        agent = CreateEntity(image, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 32, 16)
        agent.maxSpeed = 10
        agent.wanderAngle = math.random(0, 360) 
        
        CreateStateMachine(agent, wanderState)

        agents[#agents+1] = agent
    end

    agent2 = CreateEntity(image, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32, 32, 16)
    agent2.maxSpeed = 40

    CreateStateMachine(agent2, pacingState)
end

function Update()
    if gameState == 0 then
        timeLeft = timeLeft - GetFrameTime()
        if timeLeft <= 0 then
            timeLeft = 0
            gameState = 1
        else 
            for i=1, #agents do
                UpdateStateMachine(agents[i])
            end
            
            UpdateStateMachine(agent2)
        end
    end
end

function Draw()
    ClearScreen(68, 136, 204)
            
    SetDrawColor(255, 0, 255, 255)
              
    if gameState == 0 then
        for i=1, #agents do
            DrawEntity(agents[i])
        end
        DrawEntity(agent2)

        --line over agent represents direction of acceleration
        local accDirX, accDirY = Mad(agent, agent.acceleration, 32)
        DrawLine(agent.x, agent.y, accDirX, accDirY) 

        DrawText("State: " .. agent.stateMachine.currentState.name, 8, 9, font, 255, 255, 255)
        DrawText("Time Left: " .. timeLeft/1000, 8, 40, font, 255, 255, 255)
    elseif gameState == -1 then
        DrawText("You Lost!!", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, font, 255, 255, 255)
    else
        DrawText("You Won!!", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, font, 255, 255, 255)
    end
end