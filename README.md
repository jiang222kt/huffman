# 第六回の課題
以下、第六回の課題でやったことや工夫を簡潔にまとめる。

## 課題1について
- 各シンボルに対するコードを求めてツリー構造で表した。

- ツリー構造を作成する際に以下のサイトを参考にした。
https://www.techiedelight.com/c-program-print-binary-tree/

## 発展課題について
- 発展課題として圧縮ツールとして動作するように拡張した。

- 取り出すときに1byte長のbufferを用意し、変数(`byte_position`)を使ってbufferの未使用bitの箇所を明記しながら埋めていき、1byteが埋まったらそれをファイルに出力するようにした。

- インテルでは little endian のため、以下のようにメモリ上にあるシンボルのコード(整数)を2進数としてbitを取り出す方向に注意した。

```
for(int i = tmp->bits_len - 1; i >= 0; i--){
        if( byte_position <= 7){
        buffer = buffer + ( tmp->bits & ((unsigned char)1 << i));
        byte_position += 1;
    }
    if( byte_position > 7){
        fwrite(&buffer, sizeof(unsigned char), 1, fpout);
        byte_position = 0;
        buffer = buffer & 0;
    }
}
```

- 圧縮ファイルのヘッダーを確かめるためのコードである`decode.c`も作成した。これを実行すると圧縮ファイルの先頭に格納されたコードの辞書と一致することが確認できた。