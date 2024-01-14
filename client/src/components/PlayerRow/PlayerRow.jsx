import Crystals from "../Crystals/Crystals";
import MerchantCardDeck from "../MerchantCardDeck/MerchantCardDeck";
import Token from "../Token/Token";
import { IoPerson } from "react-icons/io5";
import './PlayerRow.css'


export default function PlayerRow({
    id,
    turn,
    crystals,
    merchantCardIds,
    usedMerchantCardIds,
    pointCardIds,
    numCopperTokens,
    numSilverTokens,
    active
}) {
    return (
        <div className={`player-row ${active ? 'active-player-row' : ''}`}>
            <div className='head'>
                <div>P{id}</div>
                <div className="icon">
                    <IoPerson />
                    <div>{pointCardIds.length}</div>
                </div>
            </div>
            <MerchantCardDeck numMerchantCards={merchantCardIds.length - usedMerchantCardIds.length} />
            <MerchantCardDeck numMerchantCards={usedMerchantCardIds.length} used={true} />
            <Crystals
                crystals={crystals}
            />
            <div className='silver-token'>
                <Token numTokens={numSilverTokens} color={1} />
            </div>
            <div className='copper-token'>
                <Token numTokens={numCopperTokens} color={0} />
            </div>
        </div>
    )
}