import qualified Data.Vector as V

main :: IO()
main = do
    let v = V.fromList [15, 12, 17, 44]
    let x = V.zipWith (*) (V.drop 1 v) v
    print(x)