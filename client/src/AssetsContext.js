import { createContext, useContext, useEffect, useState } from 'react'

export const AssetsContext = createContext(null)

export const useAssets = () => useContext(AssetsContext)