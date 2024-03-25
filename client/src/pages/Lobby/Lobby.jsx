import {GiGolemHead} from 'react-icons/gi'
import './Lobby.css'
import { RiVipCrownFill } from "react-icons/ri";
import UserNameModal from '../../components/UserNameModal/UserNameModal'
import { useState } from 'react'


export default function Lobby({userId, userName, hostId, id, userIdToName, onStart}) {
    const [showUserNameModal, setShowUserNameModal] = useState(false)

    const url = new URL(window.location.href)
    
    if (!url.searchParams.get('roomId')) {
        url.searchParams.append('roomId', id)
    }
    
    return (
        <div className='lobby-page page'>
            {showUserNameModal &&
                <UserNameModal originalUserName={userName} onClose={() => setShowUserNameModal(false)} />
            }
            <div className='lobby'>
                <div className='status'>
                    Invite your friends!
                </div>

                <div className='link'>
                    <input value={url} readOnly />
                    <button onClick={() => navigator.clipboard.writeText(url)}>Copy</button>
                </div>

                {userId === hostId && (
                    <button
                        onClick={onStart}
                        className='start'
                    >Start Game</button>
                )}
                
                <div className='players'>
                    {Object.entries(userIdToName).map(([id, name], i) => (
                        <div 
                            key={i}
                            className='player'
                            onClick={() => userName === name && setShowUserNameModal(true)}
                        >
                            {id === hostId && <RiVipCrownFill />}
                            {name}{name === userName ? ' (You)' : ''}
                        </div>
                    ))}
                </div>
            </div>
        </div>
    )
}
