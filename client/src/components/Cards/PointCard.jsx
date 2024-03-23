import React from 'react';
import { useAssets } from '../../AssetsContext';


export default function PointCard({id, onClick=() => {}}) {
    const [pointCards, _] = useAssets()

    return (
        <div className='card' onClick={onClick}>
            {pointCards[id]()}
        </div>
    );
}
