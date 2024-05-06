# Custom-Game-Engine

This engine is a comprehensive framework comprised of various components, each playing a pivotal role in empowering game development. Let's delve into the core elements that form the backbone of this engine.

## Datum: Dynamic Runtime Polymorphic Type

At the heart of our engine lies the Datum, a versatile data structure capable of dynamically adapting to a multitude of types during runtime. Whether it's an integer, float, string, vector, matrix, or even a custom RTTI, Datum seamlessly accommodates them all.

## Scope: Structuring Game Elements

Building upon the foundation laid by Datum, Scope introduces a hierarchical structure essential for organizing game elements effectively. These named containers provide a home for Datums, facilitating a nested arrangement conducive to scene management.

## Attribute: Bridging C++ Variables and Scopes

Attributes serve as the bridge between C++ variables and Scopes, establishing a robust two-way binding mechanism. By defining prescribed or auxiliary signatures, Attributes facilitate seamless communication between game logic and data representation.

## GameObject: Uniting Attributes and Game Logic

GameObject encapsulates game entities within the engine, extending the functionality of Attributes to incorporate essential game logic elements.

## Parse Coordinator: Generating GameObjects from JSON

The Parse Coordinator is a pivotal component enabling script-based game development within our engine. Utilizing the Chain of Responsibility pattern and the Factory pattern, it parses JSON data to dynamically create GameObject instances, offering a flexible and efficient approach to game scripting.

## Action: Enabling Dynamic Gameplay Elements

Actions represent the dynamic gameplay elements that act upon GameObjects. With a wide array of action types available, developers can implement diverse functionalities, from health increases to game state changes. Variants of actions allow for the execution of lists of actions, enhancing gameplay complexity.

## Event: Facilitating Callbacks with Observer Pattern

Events leverage the Observer pattern to enable callbacks to specific functions upon subscription. This mechanism enhances interactivity by allowing game elements to respond dynamically to various triggers and stimuli.

## Reaction: Converting Events for Action Consumption [Not included in this repo: Classmates grouped up to supercharge one engine together and make a cuphead replica in]

Reactions serve as intermediaries between Events and Actions, converting Events into Attributes to facilitate seamless integration with the Action system. This conversion process enables efficient event handling and enhances the responsiveness of gameplay mechanics.
