import FieldBoard from "./FieldBoard";
import PlayerBoard from "./PlayerBoard";


export default function Board({
    isOwnPlayer, activePointCardIds, activeMerchantCardIds,
    numSilverTokens, numCopperTokens, fieldCrystals, player,
    onActiveMerchantCardClick, onActivePointCardClick, onPlayerMerchantCardClick
}) {


    return (
        <div className='game-board'>
            <FieldBoard
                activePointCardIds={activePointCardIds}
                activeMerchantCardIds={activeMerchantCardIds}
                numSilverTokens={numSilverTokens}
                numCopperTokens={numCopperTokens}
                fieldCrystals={fieldCrystals}
                onActiveMerchantCardClick={onActiveMerchantCardClick}
                onActivePointCardClick={onActivePointCardClick}

            />
            <PlayerBoard
                isOwnPlayer={isOwnPlayer}
                {...player}

                onPlayerMerchantCardClick={onPlayerMerchantCardClick}
            />
        </div>
    )
}