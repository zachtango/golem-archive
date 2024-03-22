import { useState } from 'react'
import './Chat.css'
import { playerChat } from '../../clientMessage'


export default function Chat({chat}) {
    const [message, setMessage] = useState('')

    return (
        <div className='chat'>
            <div className='content'>
                {chat.slice().reverse().map(([type, message], i) => (
                    <div className={`message ${type} ${i % 2 ? 'odd' : 'even'}`}>
                        {message}
                    </div>
                ))}
            </div>
            <input
                placeholder='Type your message...'
                value={message}
                onChange={e => setMessage(e.target.value)}
                onKeyDown={e => {
                    if (e.key === 'Enter') {
                        playerChat(message)
                        setMessage('')
                    }
                }}
            />
        </div>
    )
}