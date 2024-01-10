import {GiGolemHead} from 'react-icons/gi'
import './Home.css'
import { randomId } from '../clientMessage'


export default function Home({onPlay}) {
    const roomId = randomId()

    return (
        <div className='home-page page'>
            <div className='logo'>
                <GiGolemHead />
            </div>
            <div className='play'>
                <button className='used'>Play vs Bots</button>
                <button onClick={() => onPlay(roomId)}>Play</button>
            </div>
        </div>
    )
}
