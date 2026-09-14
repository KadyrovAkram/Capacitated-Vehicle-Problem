import json
import os
import random

root = '/Users/alfira2002/.gemini/antigravity/scratch/cvrp-research'
instances_dir = f'{root}/data/instances'
os.makedirs(instances_dir, exist_ok=True)

configs = [
    (1, 10, 10, 200, 101),
    (2, 10, 20, 200, 102),
    (3, 10, 30, 200, 103),
    (4, 10, 40, 200, 104),
    (5, 20, 10, 400, 105),
    (6, 20, 20, 400, 106),
    (7, 20, 30, 400, 107),
    (8, 50, 10, 1000, 108),
    (9, 50, 40, 1000, 109),
    (10, 100, 20, 2000, 110),
]

for inst_id, n, q, target_demand, seed in configs:
    rng = random.Random(seed)
    depot = {"id": 0, "x": 50.0, "y": 50.0, "demand": 0}
    max_d = max(1, min(8, q - 1))
    customers = []
    total_demand = 0
    for i in range(1, n + 1):
        x = round(rng.uniform(5.0, 95.0), 2)
        y = round(rng.uniform(5.0, 95.0), 2)
        d = rng.randint(1, max_d)
        total_demand += d
        customers.append({"id": i, "x": x, "y": y, "demand": d})

    data = {
        "instance_id": inst_id,
        "name": f"Instance_{inst_id:02d}",
        "num_customers": n,
        "vehicle_capacity": q,
        "total_demand": total_demand,
        "depot": depot,
        "customers": customers,
        "sequence": list(range(1, n + 1))
    }

    filename = f"{instances_dir}/instance_{inst_id:02d}_n{n}_q{q}.json"
    with open(filename, 'w') as f:
        json.dump(data, f, indent=2)

print(f"Exported {len(configs)} benchmark instance JSON files.")
