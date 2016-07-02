{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE TypeSynonymInstances #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE TupleSections #-}

#include "vec_wrap.h"

module Main where

import Control.Exception.Base hiding (assert)
import Control.Monad (forM, unless, foldM)
import Data.Int
import Data.List (delete)
import Data.Maybe (fromMaybe)
import Foreign.Marshal.Array
import Foreign.Marshal.Utils
import Foreign.Ptr
import Foreign.Storable
import Safe (lastMay, headMay)
import Test.QuickCheck
import Test.QuickCheck.Monadic


data Cmd a = Push a
           | Init
           | Clear
           | Compact
           | Deinit
           | Extend [a]
           | First
           | Insert Int a
           | Last
           | Pop
           | Remove a
           | Reserve a
           | Reverse
           -- | Splice
           -- | SwapSplice
           -- | Swap
           | Truncate a
           deriving (Show, Eq, Ord)

data Model a = Model [a] Int32 deriving (Show, Eq, Ord)
type Model'  = Model Int32
type Cmd'    = Cmd Int32


insertAt :: a -> [a] -> Int -> Maybe [a]
insertAt x ys     0 = Just (x:ys)
insertAt x (y:ys) n = do xs <- insertAt x ys (n-1)
                         return (y:xs)
insertAt x [] n     = Nothing


runModel :: Model' -> Cmd' -> (Model', Maybe Int32)
runModel (Model xs len) cmd =
  case cmd of
    Push x      -> (Model (xs ++ [x]) (len + 1), Nothing)
    Init        -> (Model [] 0, Nothing)
    Clear       -> (Model [] 0, Nothing)
    Compact     -> (Model xs len, Nothing)
    Deinit      -> (Model [] 0, Nothing)
    Extend ys   -> (Model (xs ++ ys) (len + fromIntegral (length ys)), Nothing)
    First       -> (Model xs len, headMay xs)
    Insert ix x -> case insertAt x xs ix of
                     Nothing -> (Model xs len,       Just (-1))
                     Just xs -> (Model xs (len + 1), Just 0)
    Last        -> (Model xs len, lastMay xs)
    Pop | len == 0  -> (Model xs len, Nothing)
        | otherwise -> (Model (init xs) (len - 1), lastMay xs)
    Reserve a   -> (Model xs len, Nothing)
    Reverse     -> (Model (reverse xs) len, Nothing)
    Truncate n  -> (Model (take (fromIntegral n) xs) (min n len), Nothing)
    Remove a    -> let deleted = delete a xs
                       delLen  = length deleted
                       leng    = length xs
                   in (Model deleted (if delLen /= leng then len - 1 else len), Nothing)

runModelCmds :: [Cmd'] -> Model' -> Ptr Vec -> PropertyM IO (Model', Ptr Vec)
runModelCmds cmds model pvec = foldM step (model, pvec) cmds
  where
    step (model, pvec) cmd =
      do let (pureM, pureRes) = runModel model cmd
         (pvec', ioRes) <- run (runVec pvec cmd)
         vec' <- run (peek pvec')
         let retsMatch = ioRes == pureRes
         if not retsMatch
           then do run $ putStrLn $ show ioRes ++ " != " ++ show pureRes
                   assert retsMatch
           else assert retsMatch
         modelEquals <- run (modelEq pureM vec')
         modelAssert pureM vec' modelEquals
         return (pureM, pvec')

modelAssert m vec b =
  if b
    then assert True
    else do run (printModels m vec)
            assert False

printModels m vec = do svec <- showVec vec
                       putStrLn (show m ++ " != \n" ++ svec)

modelEq (Model xs len) vec = do
  xs' <- vdata vec
  return $ xs' == xs && len == vecLen vec

prop_api cmds = monadicIO $ do
  pvec <- run $ new (Vec nullPtr 0 0)
  runModelCmds cmds (Model [] 0) pvec


runVec :: Ptr Vec -> Cmd Int32 -> IO (Ptr Vec, Maybe Int32)
runVec pvec cmd =
  case cmd of
    Push x      -> noret (vec_push pvec x)
    Init        -> noret (vec_init pvec)
    Clear       -> noret (vec_clear pvec)
    Compact     -> noret (vec_compact pvec)
    Deinit      -> noret (vec_deinit pvec)
    Extend ys   -> noret $
                     do let len_ys = length ys
                        ys'    <- newArray ys
                        newVec <- new (Vec ys' (fromIntegral len_ys) (fromIntegral len_ys))
                        vec_extend pvec newVec
    First       -> doIfNonEmpty pvec vec_first
    Insert ix x -> do len <- vecLen <$> peek pvec
                      if ix > fromIntegral len
                        then return (pvec, Just (-1))
                        else fmap (\ret -> (pvec, Just ret))
                                  (vec_insert pvec (fromIntegral ix) x)
    Last        -> doIfNonEmpty pvec vec_last
    Pop         -> doIfNonEmpty pvec vec_pop
    Reserve a   -> noret (vec_reserve pvec a)
    Remove a    -> noret (vec_remove pvec a)
    Truncate n  -> noret (vec_truncate pvec n)
  where
    noret = fmap (, Nothing)

doIfNonEmpty pvec action = do
  len <- vecLen <$> peek pvec
  if len == 0
    then return (pvec, Nothing)
    else fmap (\ret -> (pvec, Just ret)) (action pvec)


instance Arbitrary Cmd' where
  arbitrary = do NonNegative n <- arbitrary
                 ys <- arbitrary
                 NonNegative ix <- arbitrary
                 x <- arbitrary
                 elements [ Push n, Init, Clear, Compact, Deinit, Extend ys
                          , First, Insert ix x, Last, Pop, Reserve n, Remove n
                          , Truncate n
                          ]


data Vec = Vec { vecData :: {-# UNPACK #-} !(Ptr Int32)
               , vecLen  :: {-# UNPACK #-} !Int32
               , vecCap  :: {-# UNPACK #-} !Int32
               } deriving Show

showVec :: Vec -> IO String
showVec v = do xs <- vdata v
               return $ "Vec   " ++ show xs ++ " "
                           ++ show (vecLen v) ++ " "
                           ++ show (vecCap v)

instance Storable Vec where
  sizeOf _  = (#size struct test_vec)
  alignment = sizeOf
  peek ptr = do dat    <- (#peek struct test_vec, data) ptr
                length <- (#peek struct test_vec, length) ptr
                cap    <- (#peek struct test_vec, capacity) ptr
                return (Vec dat length cap)
  poke ptr (Vec dat len cap) = do (#poke struct test_vec, data) ptr dat
                                  (#poke struct test_vec, length) ptr len
                                  (#poke struct test_vec, capacity) ptr cap


foreign import ccall "_vec_push" vec_push             :: Ptr Vec -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_init" vec_init             :: Ptr Vec -> IO (Ptr Vec)
foreign import ccall "_vec_clear" vec_clear           :: Ptr Vec -> IO (Ptr Vec)
foreign import ccall "_vec_compact" vec_compact       :: Ptr Vec -> IO (Ptr Vec)
foreign import ccall "_vec_deinit" vec_deinit         :: Ptr Vec -> IO (Ptr Vec)
foreign import ccall "_vec_extend" vec_extend         :: Ptr Vec -> Ptr Vec -> IO (Ptr Vec)
foreign import ccall "_vec_first" vec_first           :: Ptr Vec -> IO Int32
foreign import ccall "_vec_insert" vec_insert         :: Ptr Vec -> Int32 -> Int32 -> IO Int32
foreign import ccall "_vec_last" vec_last             :: Ptr Vec -> IO Int32
foreign import ccall "_vec_pop" vec_pop               :: Ptr Vec -> IO Int32
foreign import ccall "_vec_pusharr" vec_pusharr       :: Ptr Vec -> Ptr Int32 -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_remove" vec_remove         :: Ptr Vec -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_reserve" vec_reserve       :: Ptr Vec -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_reverse" vec_reverse       :: Ptr Vec -> IO (Ptr Vec)
-- foreign import ccall "_vec_sort" vec_sort             :: Ptr Vec -> , int (*fn)(const void *, const void*)
foreign import ccall "_vec_splice" vec_splice         :: Ptr Vec -> Int32 -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_swapsplice" vec_swapsplice :: Ptr Vec -> Int32 -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_swap" vec_swap             :: Ptr Vec -> Int32 -> Int32 -> IO (Ptr Vec)
foreign import ccall "_vec_truncate" vec_truncate     :: Ptr Vec -> Int32 -> IO (Ptr Vec)

main :: IO ()
main = do
  quickCheckWith (stdArgs { maxSuccess = 10000 }) prop_api

-- pushShow p n = do
--   p  <- vec_push p n
--   pv <- peek p
--   print pv
--   return p


vdata :: Vec -> IO [Int32]
vdata v = forM [0..fromIntegral (vecLen v) - 1] $ \n ->
            peekElemOff (vecData v) n
