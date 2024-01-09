import {GiGolemHead} from 'react-icons/gi'
import './Lobby.css'


export default function Lobby({onStart}) {
    const id = 'XYZWAB'
    const link = window.location.href + id

    const players = ['zazlow', 'easypete', 'bruhmomento']

    return (
        <div className='lobby-page page'>
            <div className='lobby'>
                <div className='players'>
                    <h1>Players</h1>
                    {players.map(player => (
                        <div className='player'>
                            {player}
                        </div>
                    ))}
                </div>
                <div className='game-settings'>
                    <h1>Game Settings</h1>
                    <div className='invite'>
                        <div>Invite Friends! Lobby ID: {id}</div>
                        <div className='links'>
                            <input value={link} />
                            <button onClick={() => navigator.clipboard.writeText(link)}>Copy</button>
                        </div>
                    </div>
                    <div className='controls'>
                        <button
                            onClick={onStart}
                        >Start Game</button>
                    </div>
                </div>
            </div>
        </div>
    )
}
