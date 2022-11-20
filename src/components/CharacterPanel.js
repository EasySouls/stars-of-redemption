import React, { useContext } from "react";
import { CharacterContext } from "./App";

export default function CharacterPanel(props) {
  const { character } = useContext(CharacterContext);
  const c = character;

  return (
    <div className='character-panel'>
      <b style={{ fontSize: "24px" }}>Character</b>
      <p>Name: {c.name}</p>
      <p>Level: {c.level}</p>
      <p>
        HP: {c.hpMax}/{c.hp}
      </p>
      {/*<p>Strength: {c.strength}</p>
      <p>Dexterity: {c.dexterity}</p>
      <p>Constitution: {c.constitution}</p>
      <p>Intelligence: {c.intelligence}</p>
      <p>Wisdom: {c.wisdom}</p>
  <p>Charisma: {c.charisma}</p>*/}
      <p>
        Encumbrence: {c.encumbrenceMax}/{c.encumbrence}
      </p>
    </div>
  );
}