import unittest
from studio.gain_search import suggest
from studio.tuning_trials import Trial


def trial(gains, cost, context=((1,),)):
    return Trial(tuple(gains), context, 0, 100, 2, 5, 8, 10,
                 dict(rms_error_counts=cost, overshoot_counts=0, settling_ms=0,
                      rms_demand_fraction=0))


class GainSearchTests(unittest.TestCase):
    def test_coordinate_search_preserves_limits(self):
        rows = [trial((40, 0, 10, 2, 500), 12)]
        result = suggest(rows, 'Balanced', (80, 0, 20), 25)
        self.assertEqual(result.candidate, (20, 0, 10, 2, 500))
        self.assertEqual(result.steps, (20, 0, 5))
        rows.append(trial(result.candidate, 8))
        result = suggest(rows, 'Balanced', (80, 0, 20), 25)
        self.assertEqual(result.best, (20, 0, 10, 2, 500))
        self.assertEqual(result.candidate, (0, 0, 10, 2, 500))

    def test_rejected_and_exhaustion(self):
        rows = [trial((1, 0, 0, 2, 500), 0)]
        self.assertEqual(suggest(rows, 'Precision', (2, 0, 0), 100).candidate, (0, 0, 0, 2, 500))
        rejected = [(0, 0, 0, 2, 500), (2, 0, 0, 2, 500)]
        self.assertIsNone(suggest(rows, 'Precision', (2, 0, 0), 100, rejected).candidate)

    def test_zero_gain_can_be_explored(self):
        rows = [trial((0, 0, 0, 2, 500), 10)]
        self.assertEqual(suggest(rows, 'Balanced', (0, 30, 0)).candidate, (0, 3, 0, 2, 500))

    def test_synthetic_response_converges(self):
        # Idealized convex objective; not a motor simulation.
        def measure(g):
            return trial(g, sum((a-b)**2 for a,b in zip(g[:3], (13, 4, 7))))
        rows = [measure((8, 0, 0, 2, 500))]
        for _ in range(200):
            result = suggest(rows, 'Balanced', (20, 10, 10), 25)
            if result.candidate is None:
                break
            self.assertNotIn(result.candidate, [t.gains for t in rows])
            rows.append(measure(result.candidate))
        else:
            self.fail('search did not exhaust')
        self.assertEqual(result.best, (13, 4, 7, 2, 500))

    def test_invalid_bounds_and_context(self):
        rows = [trial((40, 0, 10, 2, 500), 12)]
        for cap in ((39, 0, 10), (80, -1, 20), (80, 0, 1048577), (80, 0), (80, 0, 1.5)):
            with self.assertRaises(ValueError): suggest(rows, 'Balanced', cap)
        with self.assertRaises(ValueError): suggest(rows, 'Balanced', (80, 0, 20), 0)
        with self.assertRaises(ValueError): suggest(rows+[trial((40,0,10,2,500),1,context=((2,),))], 'Balanced',(80,0,20))
        with self.assertRaises(ValueError): suggest([], 'Balanced', (80, 0, 20))


if __name__ == '__main__':
    unittest.main()
