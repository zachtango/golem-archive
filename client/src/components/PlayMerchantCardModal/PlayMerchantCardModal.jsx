import MerchantCard from "../MerchantCard/MerchantCard";
import { IoMdClose } from "react-icons/io";
import { FaMinus, FaPlus, FaArrowDown } from "react-icons/fa";
import './PlayMerchantCardModal.css'
import merchantCards from '../../models/MerchantCards.json'
import { useState } from "react";
import Crystals from "../Crystals/Crystals";


function CrystalControls({}) {

    return (
        <div className="controls">
            <button>Play</button>
        </div>
    )
}

function UpgradeControls({numUpgrades}) {
    const [crystalsStack, setCrystalsStack] = useState([[1, 1, 1, 1]])

    function onCrystalClick(crystal) {
        const newCrystals = [...crystalsStack[crystalsStack.length - 1]]
    
        if (crystal == 3 || newCrystals[crystal] == 0 || crystalsStack.length > numUpgrades) {
            return;
        }
        
        newCrystals[crystal] -= 1;
        newCrystals[crystal + 1] += 1;

        setCrystalsStack([...crystalsStack, newCrystals])
    }

    return (
        <div className="upgrade-controls controls">
            <div className="upgrades">
                {crystalsStack.slice(0, crystalsStack.length - 1).map((crystals, i) => (
                    <div
                        key={i}
                        className="crystals-container"
                    >
                        <Crystals
                            crystals={crystals}
                        />
                        <FaArrowDown className="arrow-down"/>
                    </div>
                ))}
                <div className="crystals-container">
                    <Crystals
                        crystals={crystalsStack[crystalsStack.length - 1]}
                        onCrystalClick={onCrystalClick}
                    />
                </div>
            </div>
            {crystalsStack.length > 1 && <button>Play</button>}
        </div>
    )
}

function TradeControls({}) {
    const [numTrades, setNumTrades] = useState(0)

    function onPlus() {
        setNumTrades(numTrades + 1)
    }

    function onMinus() {
        if (numTrades > 0) {
            setNumTrades(numTrades - 1)
        }
    }

    return (
        <div className="trade-controls controls">
            <div>
                <FaPlus onClick={onPlus} />
                <div>
                    {numTrades}
                </div>
                <FaMinus onClick={onMinus} />
            </div>
            {numTrades > 0 && <button className="button">Play</button>}
        </div>
    )
}

export default function PlayMerchantCardModal({id, onClose}) {
    const merchantCard = merchantCards[id]

    let controls;

    switch(merchantCard.type) {
        case 'Crystal':
            controls = (
                <CrystalControls />
            )
            break;
        case 'Upgrade':
            controls = (
                <UpgradeControls
                    numUpgrades={merchantCard.numUpgrades}
                />
            )
            break;
        case 'Trade':
            controls = (
                <TradeControls />
            )
    }

    return (
        <div className="play-merchant-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            {controls}
        </div>
    )
}
