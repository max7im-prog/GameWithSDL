## PLAN

- Flatten render config, because current implemntation is unscalable

- Rewrite the JSONs for all sceneNodes

- Rewrite the JSON loading logic for all sceneNodes

- Rewrite the JSON loading logic for world layout (add overrides)

- Implement json schema verification

- Change literals in registering of entities to constants

- Implement human (restraints, behavior, etc.)

- Implement scene configuration through an external tool (godot???)

- Perform error handling in creation of objects in such a manner that no entity is created if any of it's components failed to create
