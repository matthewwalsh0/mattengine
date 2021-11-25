import mattengine

entity = "Cube"
speed = 10
limits = 10
direction = 1

def onUpdate(deltaTime):
    global entity, speed, limits, direction

    target = mattengine.get_entity(entity)
    target_position = target.position()

    if target_position[0] > limits:
        direction = -1
    elif target_position[0] < -limits:
        direction = 1

    x = target_position[0] + (direction * speed * deltaTime)
    y = target_position[1]
    z = target_position[2]

    target.set_position(x, y, z)