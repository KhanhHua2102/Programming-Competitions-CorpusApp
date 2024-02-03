def main():
    N, K = map(int, input().split())
    a = list(map(int, input().split()))

    fw, fl = -1, 0

    for i in range(N - 1, -1, -1):
        if a[i]:
            len_ = N - i
            fwsingle = a[i] % (K + 1) != 0 if len_ % 2 else False
            flsingle = a[i] % (K + 1) != 1 if len_ % 2 else True

            if fw == -1:
                fw, fl = fwsingle, flsingle
            else:
                fw_, fl = (not fwsingle and fw or fwsingle and fl), (not flsingle and fw or flsingle and fl)
                fw = fw_

    print("Aaron" if fw else "Bertha")

if __name__ == "__main__":
    main()
