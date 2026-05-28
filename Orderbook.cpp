#include <iostream>
#include <sys/_types/_u_int32_t.h>
#include <vector>
#include <map>
#include <algorithm>

enum class Sides{
    Asks,
    Bids
};

using Torderid  = u_int32_t;
using Tprice = double;
using Tquantity = u_int32_t;
using Ttimestamp = u_int64_t;
using TSide = Sides;

//This is the basic structure of an order.
struct Order{
    Torderid OrderID;
    Tprice Price;
    Tquantity Quantity;
    Ttimestamp Timestamp;
    TSide Side;

    //Construction Of Order
    Order(Torderid OrderID_, Tprice Price_, Tquantity Quantity_, Ttimestamp Timestamp_, TSide Side_):OrderID(OrderID_),Price(Price_), Quantity(Quantity_),Timestamp(Timestamp_), Side(Side_){}

};
using Torders = std::vector<Order*>;
//Price Level class
class PriceLevel{
    Tprice Price;
    Tquantity TotalQuantity;
    Torders Orders;
    public:
    //Construction of price level
    PriceLevel(Tprice price_, Tquantity quantity_, Torders Orders_): Price(price_), TotalQuantity(quantity_), Orders(Orders_){}
    //Adding order in the price list.

    void addOrder(Order *order){
        Orders.emplace_back(order);
        TotalQuantity += order -> Quantity;
    }

    void removeOrder(Torderid OrderId){
        for(auto it = Orders.begin(); it != Orders.end();){
            if((*it) -> OrderID == OrderId){
                TotalQuantity -= (*it) -> Quantity;
                it = Orders.erase(it);
                return;
            }else{
                ++it;
            }
        }
    }
    Order* front(){
        return (!Orders.empty())?Orders[0]:nullptr;
    }
};

using Bids = std::map<double, PriceLevel>;
using Asks = std::map<double, PriceLevel>;
using OrderMap = std:: map<Torderid, Order*>;
class OrderBook{
    Bids bids;
    Asks asks;
    OrderMap orderMap;
    // Construction of the OrderBook
    OrderBook(Bids bids_, Asks asks_, OrderMap orderMap_):bids(bids_), asks(asks_), orderMap(orderMap_){}
    // adding Orders in OrderMap
    void addOrder(Order* order){
        orderMap.insert({order->OrderID, order});
        if(order->Side == Sides:: Bids){
            bids[order-> Price].addOrder(order);
        }
        else{
            asks[order -> Price].addOrder(order);
        }
    }
    void cancelOrder(Torderid orderID){
        auto it = orderMap.find(orderID);
        // This is a sfaety check that checks whether the order exists or not.
        if(it == orderMap.end())
            return;

        Order* cancelOrder = it -> second;
        if(cancelOrder -> Side == Sides::Bids){
            bids[cancelOrder -> Price].removeOrder(orderID);
        }else{
            asks[cancelOrder -> Price].removeOrder(orderID);
        }
        orderMap.erase(it);

    }


};
