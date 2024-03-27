import { useAssets } from "../../AssetsContext";
import Spinner from "../Spinner/Spinner";
import EndStats from "./EndStats";
import FieldBoard from "./FieldBoard";
import PlayerBoard from "./PlayerBoard";


export default function Board({
    isDone,
    ownPlayer, activePointCardIds, activeMerchantCardIds,
    numSilverTokens, numCopperTokens, fieldCrystals, player, players,
    onActiveMerchantCardClick, onActivePointCardClick, onPlayerMerchantCardClick
}) {
    const assets = useAssets()
    
    let children = (
        <div className='center'>
            <Spinner />
        </div>
    )

    if (assets) {
        children = isDone ? (
            <EndStats
                ownPlayer={ownPlayer}
                players={players}
            />
        ) : (
            <>
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
                    isOwnPlayer={ownPlayer['id'] === player['id']}
                    {...player}

                    onPlayerMerchantCardClick={onPlayerMerchantCardClick}
                />
            </>
        )
    }

    return (
        <div className='game-board'>
            {children}
        </div>
    )
}