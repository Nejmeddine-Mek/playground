fibonacci :: Int -> Int
fibonacci 0 = 1
fibonacci 1 = 1
fibonacci n = fibonacci (n-1) + fibonacci (n-2)

fibs :: [Integer]
fibs = 1 : 1 : zipWith (+) fibs (tail fibs)

main:: IO ()
main = do
    print(fibonacci 5)
    print(take 6 fibs) {-We can get the fibo(5) by accessing the 6th element using: take 6 fibs !! 5 -}