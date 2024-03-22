import { useState } from "react";
import MerchantCard from "../Cards/MerchantCard";
import { IoMdClose } from "react-icons/io";
import { FaMinus, FaPlus, FaArrowDown } from "react-icons/fa";
import CrystalCount from "../CrystalCount/CrystalCount";
import merchantCards from '../../models/MerchantCards.json'
import './PlayMerchantCardModal.css'


function CrystalControls({id, onClose}) {
    // function onPlay() {
    //     onClose()
    //     crystalPlayMove(id)
    // }

    return (
        <div className="controls">
            <button>Play</button>
        </div>
    )
}

function UpgradeControls({id, numUpgrades, crystals, onClose}) {
    const [crystalsStack, setCrystalsStack] = useState([crystals])
    const [upgrades, setUpgrades] = useState([])

    function onCrystalClick(crystal) {
        const newCrystals = [...crystalsStack[crystalsStack.length - 1]]

        if (crystal == 3 || newCrystals[crystal] == 0 || crystalsStack.length > numUpgrades) {
            return;
        }
        
        newCrystals[crystal] -= 1;
        newCrystals[crystal + 1] += 1;

        setCrystalsStack([...crystalsStack, newCrystals])
        setUpgrades([...upgrades, `${crystal}${crystal + 1}`])
    }

    // function onPlay() {
    //     if (upgrades.length === 0) {
    //         return
    //     }
    //     upgradePlayMove(id, upgrades)
    //     onClose()
    // }

    return (
        <div className="upgrade-controls controls">
            <div className="upgrades">
                {crystalsStack.slice(0, crystalsStack.length - 1).map((crystals, i) => (
                    <>
                        <CrystalCount
                            crystals={crystals}
                        />
                        <FaArrowDown className="arrow-down"/>
                    </>
                ))}
                <CrystalCount
                    crystals={crystalsStack[crystalsStack.length - 1]}
                    onCrystalClick={onCrystalClick}
                />
            </div>
            <div className="buttons">
                <button>Play</button>
                <button onClick={() => {
                    setCrystalsStack([crystals])
                    setUpgrades([])
                }}>Reset</button>
            </div>
        </div>
    )
}

function TradeControls({id, fromCrystals, ownCrystals, onClose}) {
    const [numTrades, setNumTrades] = useState(0)
    
    const canPlus = fromCrystals.every((crystal, i) => ownCrystals[i] >= crystal * (numTrades + 1))

    function onPlus() {
        if (!canPlus) {
            return
        }
        setNumTrades(numTrades + 1)
    }

    function onMinus() {
        if (numTrades > 0) {
            setNumTrades(numTrades - 1)
        }
    }

    // function onPlay() {
    //     if (numTrades === 0) {
    //         return
    //     }
    //     tradePlayMove(id, numTrades)
    //     onClose()
    // }

    return (
        <div className="trade-controls controls">
            <div className="trades">
                <FaPlus onClick={onPlus} />
                <div>
                    {numTrades}
                </div>
                <FaMinus onClick={onMinus} />
            </div>
            <button className="button">Play</button>
        </div>
    )
}

export default function PlayMerchantCardModal({id, crystals, onClose}) {
    const merchantCard = merchantCards[id]

    let controls;

    switch(merchantCard.type) {
        case 'Crystal':
            controls = (
                <CrystalControls
                    id={id}
                    onClose={onClose}
                />
            )
            break;
        case 'Upgrade':
            controls = (
                <UpgradeControls
                    id={id}
                    numUpgrades={merchantCard.numUpgrades}
                    crystals={crystals}
                    onClose={onClose}
                />
            )
            break;
        case 'Trade':
            controls = (
                <TradeControls
                    id={id}
                    fromCrystals={merchantCard.fromCrystals}
                    toCrystals={merchantCard.toCrystals}
                    ownCrystals={crystals}
                    onClose={onClose}
                />
            )
    }

    return (
        <div className="play-merchant-card-modal modal center">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            {controls}
        </div>
    )
}
