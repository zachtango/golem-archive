import {GiGolemHead} from 'react-icons/gi'
import './Home.css'
import { randomId } from '../../clientMessage'
import { FaDiscord } from "react-icons/fa";
import UserNameModal from '../../components/UserNameModal/UserNameModal';
import { useState } from 'react';


export default function Home({userName, onPlay}) {
    const [showUserNameModal, setShowUserNameModal] = useState(false)
    const roomId = randomId()

    return (
        <div className='home-page page'>
            {showUserNameModal &&
                <UserNameModal originalUserName={userName} onClose={() => setShowUserNameModal(false)} />
            }
            <div onClick={() => setShowUserNameModal(true)}>
                {userName}
            </div>
            <div className='logo'>
                <GiGolemHead />
            </div>
            <div className='play'>
                <button className='used'>Play vs Bots</button>
                <button onClick={() => onPlay(roomId)}>Play</button>
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
