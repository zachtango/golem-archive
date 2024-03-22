import { useState } from 'react'
import './Chat.css'


const MESSAGE = ''
const REST = 'rest'
const ACQUIRE_MERCHANT_CARD = 'acquire-merchant-card'
const USE_MERCHANT_CARD = 'use-merchant-card'
const PURCHASE_POINT_CARD = 'purchase-point-card'


export default function Chat({chat}) {
    const [message, setMessage] = useState('')

    return (
        <div className='chat'>
            <div className='content'>
                {chat.map(([type, message], i) => (
                    <div className={`message ${type} ${i % 2 ? 'odd' : 'even'}`}>
                        {message}
                    </div>
                ))}
            </div>

            <input
                placeholder='Type your message...'
                value={message}
                onChange={e => setMessage(e.target.value)}
            >
            
            </input>
        </div>
    )
}