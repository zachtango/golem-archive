
import Logo from '../../assets/logo.svg?react'
import { randomId } from '../../clientMessage'
import { FaDiscord } from "react-icons/fa";
import { useState } from 'react';
import UserNameModal from '../../components/UserNameModal/UserNameModal'
import './Home.css'


export default function Home({userName, onPlay}) {
    const [showUserNameModal, setShowUserNameModal] = useState(false)
    const roomId = randomId()

    return (
        <div className='home-page page'>
            {showUserNameModal &&
                <UserNameModal originalUserName={userName} onClose={() => setShowUserNameModal(false)} />
            }
            <div className='logo'>
                <Logo />
            </div>
            <div className='play'>
                <div
                    className='user-name'
                    onClick={() => setShowUserNameModal(true)}
                >
                    Welcome {userName}!
                </div>
                <button onClick={() => onPlay(roomId)}>Create lobby</button>
            </div>
            <div className="footer">
                <div>
                    v1
                </div>
                <a href='https://discord.gg/6zVgtFy2ZW'>
                    <FaDiscord />
                </a>
            </div>
        </div>
    )
}
