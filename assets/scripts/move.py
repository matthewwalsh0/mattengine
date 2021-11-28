import mattengine

# string
Entity = "Cube"

# number
Speed = 1

# number
Limit = 10

direction = 1

def onUpdate(deltaTime):
    global Entity, Speed, Limit, direction

    target = mattengine.get_entity(Entity)

    if not target: return

    target_position = target.position()

    if target_position[2] > Limit:
        direction = -1
    elif target_position[2] < -Limit:
        direction = 1

    x = target_position[0]
    y = target_position[1]
    z = target_position[2] + (direction * Speed * deltaTime)

    target.set_position(x, y, z)