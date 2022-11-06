import React, { useState, useContext } from "react";
import { CharacterContext } from "./App";

export default function CharacterCreationScreen() {
  const [answer, setAnswer] = useState("");
  const [error, setError] = useState(null);
  const [status, setStatus] = useState("typing"); // 'typing', 'submitting' or 'success'

  const { character, setCharacter } = useContext(CharacterContext);

  if (status === "success") {
    setCharacter({ ...character, name: answer });
    return (
      <div className='character-creation-screen'>
        <p>Character created</p>
      </div>
    );
  }

  async function handleSubmit(e) {
    e.preventDefault();
    setStatus("submitting");
    try {
      await submitForm(answer);
      setStatus("success");
    } catch (err) {
      setStatus("typing");
      setError(err);
    }
  }

  function handleTextAreaChange(e) {
    setAnswer(e.target.value);
  }

  function submitForm(answer) {
    return new Promise((resolve, reject) => {
      setTimeout(() => {
        let shouldError = answer.toLowerCase() === ""; //! List wrong answers
        if (shouldError) {
          reject(
            new Error("You can't make a character with this name. Try again!")
          );
        } else {
          resolve();
        }
      }, 1000);
    });
  }

  return (
    <div className='character-creation-screen'>
      <h2>Choose the name of your character:</h2>
      <form onSubmit={handleSubmit}>
        <textarea
          value={answer}
          onChange={handleTextAreaChange}
          disabled={status === "submitting"}
        />
        <br />
        <button disabled={answer.length === 0 || status === "submitting"}>
          Create
        </button>
        {error !== null && (
          <p className='create-character-error'>{error.message}</p>
        )}
      </form>
    </div>
  );
}
