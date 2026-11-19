import qualified Data.Sequence as Seq 
import Data.Sequence (Seq,(|>), (<|), (><))

step_by_step_changes :: Seq Int -> Seq Int
step_by_step_changes lst = Seq.zipWith (-) (Seq.drop 1 lst) lst


main :: IO()
main = do
    let s = Seq.fromList [1,15,3,18,17]
    print(step_by_step_changes s)
    
    let s1 =  15 <| s
    let s2 = s |> 17
    let s3 = s1 >< s2
    putStrLn $ "Prepend: " ++ show s1
    putStrLn $ "Append: " ++ show s2
    putStrLn $ "Concat: " ++ show s3