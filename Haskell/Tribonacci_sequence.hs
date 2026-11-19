
tribs :: [Integer]
tribs = 1 : 1 : 2 : zipWith (+) tribs (zipWith (+) (tail tribs) (drop 2 tribs))

main :: IO()
main = 
    print(take 5 tribs)