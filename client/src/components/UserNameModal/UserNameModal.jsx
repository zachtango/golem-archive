import { useState } from 'react'
import { FaCheck } from "react-icons/fa";
import './UserNameModal.css'
import { IoMdClose } from "react-icons/io";
import { changeName } from '../../clientMessage';



export default function UserNameModal({originalUserName, onClose}) {
    const [userName, setUserName] = useState(originalUserName)

    return (
        <div className='user-name-modal'>
            <div
                className='close'
                onClick={onClose}
            >
                <IoMdClose />
            </div>
            <div className='head'>Change Username</div>

            <div className='body'>
                <input
                    value={userName}

                    onChange={e => {
                        const name = e.target.value

                        if (name.length > 10) return

                        setUserName(name)
                    }}
                ></input>
                <div onClick={() => {
                    changeName(userName)
                    onClose()
                }}>
                    <FaCheck />
                </div>
            </div>
        </div>
    )
}
