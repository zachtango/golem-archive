import YellowCrystal from '../../assets/crystals/yellowcrystal.svg?react'
import GreenCrystal from '../../assets/crystals/greencrystal.svg?react'
import BlueCrystal from '../../assets/crystals/bluecrystal.svg?react'
import PinkCrystal from '../../assets/crystals/pinkcrystal.svg?react'
import './CrystalDisplay.css'


export default function CrystalDisplay({crystals, totalCrystals}) {

    const crystalsLeft = totalCrystals ? [...Array(Math.max(0, totalCrystals - crystals.reduce((accum, value) => accum + value, 0))).keys().map(() => (
        <YellowCrystal style={{opacity: '40%'}} />
    ))] : []

    const yellows = [...Array(crystals[0]).keys()].map(() => (
        <YellowCrystal />
    ))
    const greens = [...Array(crystals[1]).keys()].map(() => (
        <GreenCrystal />
    ))
    const blues = [...Array(crystals[2]).keys()].map(() => (
        <BlueCrystal />
    ))
    const pinks = [...Array(crystals[3]).keys()].map(() => (
        <PinkCrystal />
    ))

    return (
        <div className='crystal-display'>
            {...crystalsLeft}
            {...yellows}
            {...greens}
            {...blues}
            {...pinks}
        </div>
    )
}
