IEnumerable<int> queryHighScores = from score in Scores
                                    where score > 80
                                    select score;
