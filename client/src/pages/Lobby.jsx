import {GiGolemHead} from 'react-icons/gi'
import './Lobby.css'
import { randomId } from '../clientMessage'


export default function Lobby({userId, hostId, id, userIds, onStart}) {
    
    const url = new URL(window.location.href)
    
    if (!url.searchParams.get('roomId')) {
        url.searchParams.append('roomId', id)
    }

    return (
        <div className='lobby-page page'>
            <div className='lobby'>
                <div className='players'>
                    <h1>Players</h1>
                    {userIds.map((id, i) => (
                        <div 
                            key={i}
                            className='player'
                        >
                            P{id}
                        </div>
                    ))}
                </div>
                <div className='game-settings'>
                    <h1>Game Settings</h1>
                    <div className='invite'>
                        <div>Invite Friends! Lobby ID: {id}</div>
                        <div className='links'>
                            <input value={url} readOnly />
                            <button onClick={() => navigator.clipboard.writeText(url)}>Copy</button>
                        </div>
                    </div>
                    <div className='controls'>
                        {userId === hostId && (
                            <button
                                onClick={onStart}
                                className={`${userIds.length < 2 ? 'used' : ''}`}
                            >Start Game</button>
                        )}
                    </div>
                </div>
            </div>
        </div>
    )
}
