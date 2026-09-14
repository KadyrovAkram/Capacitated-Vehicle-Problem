#!/usr/bin/env python3
import subprocess
import os
import sys

root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
bin_path = os.path.join(root, 'bin', 'cvrp_solver')

def main():
    if not os.path.exists(bin_path):
        print("[INFO] Binary not found. Building with make...")
        subprocess.check_call(['make', '-C', root])

    csv_output = os.path.join(root, 'data', 'benchmark_results.csv')
    cmd = [bin_path, '--all', '--export-csv', csv_output]

    print(f"[INFO] Executing benchmark suite: {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=root, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    print(result.stdout)

    if result.returncode != 0:
        print(f"[ERROR] Benchmark failed with return code {result.returncode}")
        print(result.stderr)
        sys.exit(result.returncode)

    print("[SUCCESS] Benchmark run completed successfully!")

if __name__ == '__main__':
    main()
