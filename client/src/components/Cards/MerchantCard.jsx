import React from 'react';
import { useAssets } from '../../AssetsContext';

export default function MerchantCard({ id, used=false, onClick=() => {} }) {
    const [_, merchantCards] = useAssets()

    return (
        <div
            className='card'
            onClick={onClick}
            style={{
                opacity: !used ? 1 : 0.5
            }}    
        >
            {merchantCards[id]()}
        </div>
    );
}
