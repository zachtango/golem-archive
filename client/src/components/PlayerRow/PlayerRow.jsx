import Crystals from "../Crystals/Crystals";
import MerchantCardDeck from "../MerchantCardDeck/MerchantCardDeck";
import Token from "../Token/Token";
import { IoPerson } from "react-icons/io5";
import './PlayerRow.css'


export default function PlayerRow() {
    const playerName = 'Player 1'
    const numGolems = 5;
    return (
        <div className='player-row'>
            <div className='head'>
                <div>{playerName}</div>
                <div className="icon">
                    <IoPerson />
                    <div>{numGolems}</div>
                </div>
            </div>
            <MerchantCardDeck numMerchantCards={10} />
            <MerchantCardDeck numMerchantCards={10} used={true} />
            <Crystals
                crystals={[1, 1, 0, 2]}
            />
            <div className='copper-token'>
                <Token numTokens={1} color={1} />
            </div>
            <div className='silver-token'>
                <Token numTokens={1} color={0} />
            </div>
        </div>
    )
}