import {GiGolemHead} from 'react-icons/gi'
import './Home.css'


export default function Home({onPlay}) {
    return (
        <div className='home-page page'>
            <div className='logo'>
                <GiGolemHead />
            </div>
            <div className='play'>
                <button className='used'>Play vs Bots</button>
                <button onClick={onPlay}>Play</button>
            </div>
        </div>
    )
}
