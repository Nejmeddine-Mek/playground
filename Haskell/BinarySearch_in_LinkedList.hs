binarySearch :: [Int] -> Int -> Int -> Int -> Int
binarySearch table target start end
    | start > end = -1
    | table !! middle < target = binarySearch table target (middle + 1) end 
    | table !! middle > target = binarySearch table target start (middle - 1)
    | table !! middle == target = middle
        where
            middle = (start + end) `div` 2