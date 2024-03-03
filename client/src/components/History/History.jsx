import { useState } from 'react';
import './History.css'
import { VscSend } from "react-icons/vsc";
import { playerChat } from '../../clientMessage';

export default function History({history}) {
    const [message, setMessage] = useState('')

    return (
        <div className="history">
            <div className='content'>
                {history.slice().reverse().map((action, i) => (
                    <div
                        key={i}
                        className='action'
                    >
                        {action}
                    </div>
                ))}
            </div>
            <div className='chat'>
                <input
                    placeholder='Send a message...'
                    value={message}

                    onKeyDown={(e) => {
                        if (e.key !== 'Enter') return

                        playerChat(message)
                        setMessage('')
                    }}
                    onChange={e => setMessage(e.target.value)}
                ></input>
                
                <div
                    className='send'
                    onClick={() => {playerChat(message), setMessage('')}}
                >
                    <VscSend />
                </div>
            </div>
        </div>
    )
}
