import Crystals from "../Crystals/Crystals";
import MerchantCard from "../MerchantCard/MerchantCard";
import PointCard from "../PointCard/PointCard";
import Token from "../Token/Token";
import { IoIosRefresh } from "react-icons/io";
import './PlayerBoard.css'


export default function PlayerBoard({onMerchantCardClick}) {
    const pointCardIds = [0, 1, 2, 3, 4, 5]
  
    const playerMerchantCardIds = [0, 1]
  
    return (
        <div className='player-board'>
            <div className='head'>
                <div className='crystals-container'>
                    <Crystals
                        crystals={[1, 1, 1, 0]}
                    />
                </div>
                <div className='token-container'>
                    <div className='copper-token'>
                        <Token numTokens={1} color={1} />
                    </div>
                </div>
                <div className='token-container'>
                    <div className='silver-token'>
                        <Token numTokens={1} color={0} />
                    </div>
                </div>
                <div className='point-cards-container'>
                    <div className='point-cards'>
                        {pointCardIds.map(id => (
                            <PointCard key={id} id={id} />
                        ))}
                    </div>
                </div>
                <div className='turn-status-container' style={{display: 'none'}}>
                    <div className='turn-status'>P1's turn...</div>
                </div>
            </div>
            <div className='body'>
                <div className='merchant-cards-container'>
                    <div className='merchant-cards'>
                        {playerMerchantCardIds.map(id => (
                            <MerchantCard
                                key={id}
                                id={id}
                                onClick={() => onMerchantCardClick(id)}
                            />
                        ))}
                    </div>
                </div>
                <div className='rest-container'>
                    <div className="rest">
                        <IoIosRefresh />
                    </div>
                </div>
            </div>
        </div>
    )
}