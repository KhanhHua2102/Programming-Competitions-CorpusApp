import unittest
from helper import get_tex_path, get_label_from_path, is_valid_label, extract_contest_data, escape_special_characters

class Test(unittest.TestCase):

    def test_get_label_from_path_WINDOWS(self):
        self.assertEqual(get_label_from_path("C:\\Users\\YourUsername\\Documents\\example.txt"), "example.txt")
        self.assertEqual(get_label_from_path("D:\\Projects\\ProjectFolder\\data\\file.csv"), "file.csv")

    def test_get_label_from_path_UNIX(self):
        self.assertEqual(get_label_from_path("/home/YourUsername/Documents/example.txt"), "example.txt")
        self.assertEqual(get_label_from_path("/mnt/data/photos/photo.jpg"), "photo.jpg")

    def test_is_valid_label(self):
        self.assertEqual(is_valid_label('A'), True)
        self.assertEqual(is_valid_label('Z'), True)
        self.assertEqual(is_valid_label('F'), True)
        self.assertEqual(is_valid_label('A '), False)
        self.assertEqual(is_valid_label('b'), False)
        self.assertEqual(is_valid_label('c\\u6'), False)
        self.assertEqual(is_valid_label('\n'), False)
        self.assertEqual(is_valid_label(' '), False)

    def test_extract_contest_tex_data(self):
        self.assertEqual(extract_contest_data("testing/xxRegionals"), {'A': {'problemname': 'A Menace 2 \\$ociety'}, 
                                                                        'B': {'problemname': 'Begrudging Friendship 2'}, 
                                                                        'C': {'problemname': 'Cakes and Ogres'}, 
                                                                        'D': {'problemname': 'Distinct Platforms'}, 
                                                                        'E': {'problemname': 'Emergency Plan'}, 
                                                                        'F': {'problemname': 'Finding Clusters'}, 
                                                                        'G': {'problemname': 'Great Trails'}, 
                                                                        'H': {'problemname': 'Holy Stone of Destiny'}, 
                                                                        'I': {'problemname': 'Iguana Honeymoon'}, 
                                                                        'J': {'problemname': "Jay and Jay's son ja\\$on"}, 
                                                                        'K': {'problemname': 'Keeping Arrays Menacing'}, 
                                                                        'L': {'problemname': 'Land Tax'}})
        self.assertEqual(extract_contest_data("testing/regionals2023"), {'A': {'problemname': 'Attention Stones'}, 
                                                                         'B': {'problemname': 'Bingo Boards'}, 
                                                                         'C': {'problemname': 'Cave Escape'}, 
                                                                         'D': {'problemname': 'Door to Treasure'}, 
                                                                         'E': {'problemname': 'Eating Bacteria'}, 
                                                                         'F': {'problemname': 'Flawed LLM'}, 
                                                                         'G': {'problemname': 'Gem Machine'}, 
                                                                         'H': {'problemname': 'Hiring an Accountant'}, 
                                                                         'I': {'problemname': 'Investigator of Crashes'}, 
                                                                         'J': {'problemname': "Jay's Son Ja\\$on's Family Tree"}, 
                                                                         'K': {'problemname': 'Killing Monsters'}, 
                                                                         'L': {'problemname': 'Lonely Wizard'}})

    def test_escape_special_characters(self):
        self.assertEqual(escape_special_characters("""A string $s_1s_2 \cdots s_n$ is lexicographically smaller than $t_1t_2\cdots t_\ell$ if

    \begin{itemize}
    \item there exists $k \leq \min(n,\ell)$ such that $s_i = t_i$ for all $1 \leq i < k$ and $s_k < t_k$
    \end{itemize}

    \vskip-10pt
    \hskip30pt or

    \begin{itemize}
    \item \vskip-10pt $s_i = t_i$ for all $1 \leq i \leq \min(n,\ell)$ and $n < \ell$.
    \end{itemize}"""),r"""A string $s_1s_2 \cdots s_n$ is lexicographically smaller than $t_1t_2\cdots t_\ell$ if

    \begin{itemize}
    \item there exists $k \leq \min(n,\ell)$ such that $s_i = t_i$ for all $1 \leq i < k$ and $s_k < t_k$
    \end{itemize}

    \vskip-10pt
    \hskip30pt or

    \begin{itemize}
    \item \vskip-10pt $s_i = t_i$ for all $1 \leq i \leq \min(n,\ell)$ and $n < \ell$.
    \end{itemize}""")

if __name__ == '__main__':
    unittest.main()
