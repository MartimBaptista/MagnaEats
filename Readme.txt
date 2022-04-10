Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
    Realizado por: Cosmin Trandafir Nº 57101
                   João Serafim     Nº 56376
                   Martim Baptista  Nº 56273


Antes da execução, deve-se com um terminal fazer "make" na diretoria MagnaEats (os ficheiros criados podem ser removidos através de "make clean").
Para executar o programa deve-se correr ./bin/magnaeats (max_ops) (buffer_size) (n_restaurants) (n_drivers) (n_client).
Para evitar muitos erros devido à sincronização não estar implementada recomendamos que não existam mais do que um "driver".
Erros devido à falta de sincronização:
    -Múltiplos drivers podem ler o mesmo pedido.
    -Pode acontecer que o pointer "out" do circular buffer passe à frente do "in" devido a múltiplos drivers alterarem o valor simultaneamente,
    isto provoca que aproxima leitura de uma "volta" ao buffer.
    -Os valores apresentados no status podem estar incorretos, apesar de a execução estar bem, isto deve se a por exemplo, o driver mandar o pedido
    ao cliente, o cliente atualiza o results (onde o status vai buscar), e só depois o driver atualiza os valores do results, dando overwrite
    na informação (mais atualizada) do cliente.