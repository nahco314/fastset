from typing import Generic, TypeVar, Union, List, Tuple, Optional


T = TypeVar("T")


class Set(Generic[T]):
    # 参考: https://github.com/Lgeu/snippet/blob/master/cpp_extension/wrap_cpp_set.py
    # 参考: https://nagiss.hateblo.jp/entry/2020/09/08/203701

    def __init__(self, lst: Union[List[T], Tuple[T]] = None):
        """
        ・ 引数無しで呼ぶと、空の Set オブジェクトを作る
        ・ リストまたはタプルを引数に入れると、その要素が入った Set オブジェクトを作る
        ・ 計算量は O(n log n)
        　 ・ リストがソート済みの場合は O(n)
        ・ イテレータは最小の要素を指す
        """
        ...

    def add(self, x: T) -> bool:
        """
        ・ 要素を追加して True を返す
        ・ ただし、既に同じ要素が入っていた場合、要素を追加せず False を返す
        ・ 計算量は O(log n)
        ・ イテレータは追加した要素（または既に入っていた要素）を指す
        """
        ...

    def remove(self, x: T) -> Optional[T]:
        """
        ・ 指定した要素を削除して、削除した要素の次の要素を返す
        　 ・ 最も大きい要素を削除した場合、None を返す
        ・ 計算量は O(log n)
        ・ イテレータは返した要素を指す
        　 ・ None を返した場合、最大の要素の次を指す
        ・ 指定した要素が入っていなければ KeyError を出す
        """
        ...

    def search_higher_equal(self, x: T) -> Optional[T]:
        """
        ・ 指定した要素と同じかそれより大きい要素を返す
        　 ・ そのような要素がなければ None を返す
        ・ 計算量は O(logn)
        ・ イテレータは返した要素を指す
        　 ・ None を返した場合、最大の要素の次を指す
        """
        ...

    def min(self) -> T:
        """
        ・ 最小の要素を返す
        ・ イテレータは返した要素を指す
        ・ 計算量は O(1)
        ・ 要素数が 0 の場合、 IndexError を出す
        """
        ...

    def max(self) -> T:
        """
        ・ 最大の要素を返す
        ・ イテレータは返した要素を指す
        ・ 計算量は O(1)
        ・ 要素数が 0 の場合、 IndexError を出す
        """
        ...

    def pop_min(self) -> T:
        """
        ・ 最小の要素を削除し、その値を返す
        ・ イテレータは削除した要素の次の要素を指す
        　 ・ すなわち、削除後の最小の要素を指す
        ・ 計算量は O(1)
        ・ 要素数が 0 の場合、 IndexError を出す
        """
        ...

    def pop_max(self) -> T:
        """
        ・ 最大の要素を削除し、その値を返す
        ・ イテレータは削除した要素の次の要素を指す
        　 ・ すなわち、削除後の最大の要素を指す
        ・ 計算量は O(1)
        ・ 要素数が 0 の場合、 IndexError を出す
        """
        ...

    def __len__(self) -> int:
        """
        ・ 要素数を返す
        ・ 計算量は O(1)
        """
        ...

    def next(self) -> Optional[T]:
        """
        ・ イテレータをひとつ進めた後、イテレータの指す要素を返す
        ・ イテレータが最大の要素を指していた場合、イテレータをひとつ進め、 None を返す
        ・ イテレータが最大の要素の次を指していた場合、イテレータを動かさず、 None を返す
        ・ 計算量は O(1)
        """
        ...

    def prev(self) -> Optional[T]:
        """
        ・ イテレータをひとつ戻した後、イテレータの指す要素を返す
        ・ イテレータが最小の要素を指していた場合、イテレータを動かさず、 None を返す
        ・ 計算量は O(1)
        """
        ...

    def to_list(self) -> List[T]:
        """
        ・ 要素をリストにして返す
        ・ 計算量は O(n)
        """
        ...

    def get(self) -> Optional[T]:
        """
        ・ イテレータの指す要素を返す
        ・ イテレータが最大の要素を指していた場合、 None を返す
        ・ 計算量は O(1)
        """
        ...

    def erase(self) -> Optional[T]:
        """
        ・ イテレータの指す要素を削除し、その次の要素を返す
        　 ・ そのような要素がなければ None を返す
        ・ イテレータは返した要素を指す
        　 ・ None を返した場合、最大の要素の次を指す
        ・ 計算量は O(1)
        ・ イテレータが最大の要素の次を指していた場合、 KeyError を出す
        """
        ...

    def __getitem__(self, item: int) -> T:
        """
        ・ k 番目の要素を返す
        　 ・ 負の値もいける
        ・ 計算量は O(log n)
        ・ イテレータは返した要素を指す
        　 ・ g++ 環境でない場合、計算量は O(n)
        ・ k 番目の要素が存在しない場合、 IndexError を出す
        """
        ...

    def pop(self, k: int) -> T:
        """
        ・ k 番目の要素を削除し、その値を返す
        　 ・ 負の値もいける
        ・ 計算量は O(log n)
        ・ イテレータは返した要素の次の要素を指す
        　 ・ g++ 環境でない場合、計算量は O(n)
        ・ k 番目の要素が存在しない場合、 IndexError を出す
        """
        ...

    def index(self, x: T) -> int:
        """
        ・ 要は bisect_left
        ・ 計算量は O(log n)
        　 ・ g++ 環境でない場合、計算量は O(n)
        ・ イテレータは変化しない
        """
        ...

    def copy(self) -> "Set[T]":
        """
        ・ 自身のコピーを返す
        ・ 計算量は O(n)
        ・ 新しいオブジェクトのイテレータは最初の要素を指す
        """
        ...
